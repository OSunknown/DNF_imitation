#include "stdafx.h"
#include "MasterScene.h"
#include "BackgroundSpriteObject.h"
#include "BackgroundAniObject.h"
#include "CharacterSelectButtonObject.h"
MasterScene::MasterScene()
{
	_currentScene = sceneNames::None;
	_topLimit = 0;
	_LeftLimit = 0;
	_RightLimit = 0;
	_ButtomLimit = 0;
	sceneLoadOk = false;
	sceneUpdateLock = true;
}


MasterScene::~MasterScene()
{
}

HRESULT MasterScene::Init(void)
{
	//ChangeScene(sceneNames::Scene_Elvenguard);
	ChangeScene(sceneNames::Scene_CharacterSelect);
	return 1;
}

void MasterScene::Shutdown(void)
{
	if ((int)_currentScene)
	{
		for (size_t i = 0; i < Background.size(); i++)
		{
			delete Background[i];
		}
	}
}

void MasterScene::Update(void)
{

}

void MasterScene::FrameUpdate(void)
{
	if (sceneUpdateLock == true) return;
	if (sceneLoadOk == false) return;
	if (_currentScene == sceneNames::None) return;



	PCController.FixedUpdate();
	//Render 프레임별로 도는 애니메이션 랜더 + 기본 바탕 뿌려줄것.
	DxM.TurnZBufferOff();

	for (size_t i = 0; i < Background.size(); i++)
	{
		Background[i]->Frame();
	}
	DxM.TurnZBufferOn();

	//케릭터 선택창의 케릭터가 그려지고 있어야한다.

	for (size_t i = 0; i < GameObjects.size(); i++)
	{
		GameObjects[i]->Frame();
	}
	DxM.TurnZBufferOn();
	//맵 상에 몬스터가 남았다면..
	//Player._HereIsAMonster = true;
	//player render
	//Player.Render();


	DxM.TurnZBufferOff();
	for (size_t i = 0; i < Nearground.size(); i++)
	{
		Nearground[i]->Frame();
	}
	DxM.TurnZBufferOn();


	DxM.TurnZBufferOff();
	for (size_t i = 0; i < UI.size(); i++)
	{
		UI[i]->Frame();
	}
	DxM.TurnZBufferOn();

	//MainCamera.AddPosition(_moveInput.x, _moveInput.y, 0);
	//로딩이미지가 마지막에 있으면 모두 덮고 그려 질 수 있겠지.

}

HRESULT MasterScene::ChangeScene(sceneNames sceneNumber)
{
	sceneLoadOk = false;
	//로딩 이미지 켜기
	
	for (int i = 1; i < UI.size(); i++)
	{
		UI[UI.size() - i]->Shutdown();
	}
	UI.clear();
	switch (sceneNumber)
	{
	case sceneNames::None:
		break;
	case sceneNames::Scene_CharacterSelect:
		_currentScene = sceneNames::Scene_CharacterSelect;
		LoadScene_CharacterSelect();
		break;
	case sceneNames::Scene_Elvenguard:
		_currentScene = sceneNames::Scene_Elvenguard;
		MainCamera.SetPostion(0, 0, 0);//카메라 위치 바꾸기.
		LoadScene_Elvenguard();
		break;
	}

	sceneLoadOk = true;
	return E_NOTIMPL;
}

void MasterScene::AddCharacter(int slotNum, D3DXVECTOR2 position)
{
	BaseCharacterObject* bco;

	bco = new BaseCharacterObject();
	bco->init(slotNum);
	bco->SetUiType();
	bco->SetPosition(position + D3DXVECTOR2(0,-50));
	GameObjects.push_back(bco);
}

void MasterScene::LoadScene_CharacterSelect()
{
	//IMAGEMANAGER.LoadImagePack(1);
	SetScreenLimit(0, 0, 0, 0); //화면이 좌우로 움직일 필요가 없으므로
	//배경과 버튼들
	//BackgroundSpriteObject* sgo;
	//MainCamera.SetPostion(App.DxscreenWidth * 0.5f, App.DxscreenHeight * 0.5f, 0);
	CharacterSelectButtonObject* CSBtn;
	int index = 0;
	//UI 타입이니까 화면 기준으로 위치 잡아주세요
	for (int i = 1; i > -1; i--)
	{
		for (int j = -2; j <= 2; j++)
		{
			CSBtn = new CharacterSelectButtonObject();
			CSBtn->Init(index, j * 110.0f +(App.DxscreenWidth*0.5f), (App.DxscreenHeight * 0.5f)+90-(i * 180.0f), 100, 160);
			index++;
			UI.push_back(CSBtn);
		}
	}
}

void MasterScene::LoadScene_Elvenguard()
{
	BackgroundSpriteObject* sgo;
	BackgroundAniObject* anigo;

	SetScreenLimit(280, -1254, 1254, -280);
	//Pack 로드 
	IMAGEMANAGER.LoadImagePack(1);

	//가장 먼 배경
	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 168), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("back_far_0", 50);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1043.6f,0, 0)
		,D3DXVECTOR3(-549,	  0, 0)
		,D3DXVECTOR3(-128.1f, 0, 0)
		,D3DXVECTOR3(293.19f, 0, 0)
		,D3DXVECTOR3(714.48f, 0, 0)
		,D3DXVECTOR3(1018.5f, 0, 0) });
	Background.push_back(sgo);


	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(886.5f, 151), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("back_far_2", 49);
	sgo->InitializeBuffers();
	sgo->InitializeInstance({ D3DXVECTOR3(0, 0, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.95f, 0.95f, 1));
	sgo->SetTexture("back_middle_0", 48);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-931.7f, 121.2f, 0)
		, D3DXVECTOR3(-522.9f, 120.9f, 0)
		,D3DXVECTOR3(-68.6f, 122.3f, 0)
		,D3DXVECTOR3(390.1f, 123.7f, 0)
		,D3DXVECTOR3(1353, 120.9f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(-894, 151.3f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("back_far_1", 47);
	sgo->InitializeBuffers();
	sgo->InitializeInstance({ D3DXVECTOR3(0, 0, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.95f, 0.95f, 0.95f));
	sgo->SetTexture("near_1", 48);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(972, 2.12f, 0)
		, D3DXVECTOR3(807.2f, 10.96f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.935f, 0.935f, 1));
	sgo->SetTexture("tile_0", 47);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1150,	  -113.84f, 0)
		, D3DXVECTOR3(-940.56f,-113.84f, 0)
		, D3DXVECTOR3(-731.5f, -114.27f, 0)
		, D3DXVECTOR3(-523.1f, -113.84f, 0)
		, D3DXVECTOR3(-314.2f, -113.84f, 0)
		, D3DXVECTOR3(-104.9f, -113.84f, 0)
		, D3DXVECTOR3(104,	  -113.84f, 0)
		, D3DXVECTOR3(312.9f,  -113.84f, 0)
		, D3DXVECTOR3(522.3f,  -114.55f, 0)
		, D3DXVECTOR3(731.5f,  -113.84f, 0)
		, D3DXVECTOR3(940.8f,  -113.84f, 0)
		, D3DXVECTOR3(1149.5f, -113.84f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, -260.2153f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.935f, 0.935f, 1));
	sgo->SetTexture("tile_add40_0", 47);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1150.0f,0, 0)
		, D3DXVECTOR3(-940.56f,0, 0)
		, D3DXVECTOR3(-731.5f, 0, 0)
		, D3DXVECTOR3(-523.1f, 0, 0)
		, D3DXVECTOR3(-314.2f, 0, 0)
		, D3DXVECTOR3(-104.9f, 0, 0)
		, D3DXVECTOR3(104.0f,  0, 0)
		, D3DXVECTOR3(312.9f,  0, 0)
		, D3DXVECTOR3(522.3f,  0, 0)
		, D3DXVECTOR3(731.5f,  0, 0)
		, D3DXVECTOR3(940.8f,  0, 0)
		, D3DXVECTOR3(1149.5f, 0, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_5", 46);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(66.3f, -167.6f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_0", 45);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1081.0f,-122.33f, 0)
		, D3DXVECTOR3(-945.5f,-167.58f, 0)
		, D3DXVECTOR3(204.6f, -161.9f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_2", 44);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1181, -173.95f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_1", 43);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-849.6f, -126.57f, 0)
		, D3DXVECTOR3(202.8f, -99.7f, 0)
		, D3DXVECTOR3(154, -79.2f, 0)
		, D3DXVECTOR3(435, -177.5f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_3", 42);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-770.3f, -161.9f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_4", 41);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-679.4f, -154.85f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_5", 40);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-747.6f, -111.7f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_1", 39);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-601.6f, -101.1f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_3", 38);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-575.3f, -141.4f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_0", 37);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-382.2f, -156.2f, 0)
		, D3DXVECTOR3(556.6f, -135.76f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_4", 36);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-156.1f, -174.6f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_1", 35);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(763.9f, -132.9f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("block_5", 34);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(849.2f, -72,0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(-362.3f, 118.79f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.933f, 0.933f, 1));
	sgo->SetTexture("blacksmith_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(0, 0,0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("tree_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1067, 149.2f,0)
		, D3DXVECTOR3(1095.8f, 144.2f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("tree_1", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(1229, 135.9f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("tent_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1149, 21.9f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.931f, 0.931f, 1));
	sgo->SetTexture("seria_tree_n_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-794.5f, 95.7f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("sign_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-494, 76.37f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.935f, 0.935f, 1));
	sgo->SetTexture("myroom_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(557.4f, 110.3f, 0) });
	Background.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.935f, 0.935f, 1));
	sgo->SetTexture("sign_dungeon_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1060.5f, -2.4f, 0) });
	Background.push_back(sgo);

	anigo = new BackgroundAniObject();
	anigo->SetTransform(D3DXVECTOR2(-292.5f, 66.3f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.94f, 0.94f, 1));
	anigo->SetAnimation("blacksmith", 33);
	anigo->InitializeBuffers();
	Background.push_back(anigo);

	anigo = new BackgroundAniObject();
	anigo->SetTransform(D3DXVECTOR2(-350, 300), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	anigo->SetAnimation("blacksmith_smoke", 33);
	anigo->InitializeBuffers();
	Background.push_back(anigo);

	////여기서 부터 플레이어보다 앞에 있음.

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.935f, 0.935f, 1));
	sgo->SetTexture("sign_dungeon_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-1150, -186.4f, 0) });
	Nearground.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("near_0", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-847.6f, -218.35f, 0)
		, D3DXVECTOR3(752.7f, -233.9f, 0) });
	Nearground.push_back(sgo);

	sgo = new BackgroundSpriteObject();
	sgo->SetTransform(D3DXVECTOR2(0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.93f, 0.93f, 1));
	sgo->SetTexture("near_1", 33);
	sgo->InitializeBuffers();
	sgo->InitializeInstance(
		{ D3DXVECTOR3(-782, -240.4f, 0)
		, D3DXVECTOR3(-989.2f, -241.8f,0)
		, D3DXVECTOR3(-869.5f, -283.4f, 0)
		, D3DXVECTOR3(-456.5f, -267.4f, 0)
		, D3DXVECTOR3(-87.7f, -249.7f, 0)
		, D3DXVECTOR3(219.4f, -269, 0)
		, D3DXVECTOR3(219.4f, -269, 0)
		, D3DXVECTOR3(1133.9f, -220.9f,0)
		, D3DXVECTOR3(1059.6f, -260.6f, 0)
		, D3DXVECTOR3(1016.3f, -235.3f, 0)
		, D3DXVECTOR3(1191.4f, -269, 0)
		, D3DXVECTOR3(973.1f, -278, 0)
		, D3DXVECTOR3(840.6f, -273.5f, 0) });
	Nearground.push_back(sgo);
}

float MasterScene::Top()
{
	return _topLimit;
}

float MasterScene::Left()
{
	return _LeftLimit;
}

float MasterScene::Right()
{
	return _RightLimit;
}

float MasterScene::Buttom()
{
	return _ButtomLimit;
}

void MasterScene::SetScreenLimit(float top, float left, float right, float buttom)
{
	_topLimit = top;
	_LeftLimit = left;
	_RightLimit = right;
	_ButtomLimit = buttom;
}
