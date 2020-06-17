#include "stdafx.h"
#include "ImageManager.h"
#include <D3DX11tex.h>
#include "Animator.h"


ImageManager::ImageManager()
{
	_currentPackID = 0;
}


ImageManager::~ImageManager()
{
}

void ImageManager::Init()
{
	_currentPackID = 0;
	//UI와 Home(세리아방)은 계속 로드 되어있어야 하니 
	//미리미리 불러둡시다.
	LoadUI();
	LoadSeriaRoom();
	LoadPlayableCharacterPack();
}

void ImageManager::Shutdown()
{
	_mSceneSprites.clear();
}

HRESULT ImageManager::LoadImagePack(int PackID)
{

	if (_currentPackID == PackID)
	{
		return TRUE;
	}
	_currentPackID = PackID;
	switch (PackID)
	{
	case 1:
		LoadBelMyrePrincipality_South();
		break;
	}
	return TRUE;
}

sprite* ImageManager::GetSceneSprite(string name)
{
	if (_mSceneSprites.find(name) == _mSceneSprites.end())
	{
		MessageBox(DxM.GethWnd(), "Sprite를 못찾습니다", "GetSceneSprite", 0);
		return NULL;
	}

	return _mSceneSprites[name];
}

sprite * ImageManager::GetSeriaSprite(string name)
{
	if (_mSeriaSprites.find(name) == _mSeriaSprites.end())
	{
		MessageBox(DxM.GethWnd(), "Sprite를 못찾습니다", "GetSeriaSprite", 0);
		return NULL;
	}

	return _mSeriaSprites[name];
}

sprite * ImageManager::GetUISprite(string name)
{
	if (_mUISprites.find(name) == _mUISprites.end())
	{
		//뭔가 문제가 있어요.
		return NULL;
	}

	return _mUISprites[name];
}
Animation * ImageManager::GetBgAnimation(string name)
{
	return _mBackgroundGifs[name];
}
Animator ImageManager::GetAnimator(string name)
{
	if (_mAnimatorControlls.find(name) == _mAnimatorControlls.end())
	{
		//뭔가 문제가 있어요.
		printf("%s 는 없거나 무언가 문제가 있는 GetAnimation", name.c_str() );
	}
	return _mAnimatorControlls[name];
}
//  고정형 오브젝트 생성에 쓰는 이미지 이므로 완전 기본형 offset도 pivot도 
void ImageManager::AddSprite(unordered_map<string, sprite*> & pack, string name, string path, float width, float height)
{
	AddSprite(pack, name, path, width, height, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0,0));
}
// 고정형 오브젝트 생성에 쓰는 이미지 이므로 offset만 사용 pivot은 사용하지 않는다.
void ImageManager::AddSprite(unordered_map<string, sprite*>& pack, string name, string path, float width, float height, D3DXVECTOR2 offset)
{
	AddSprite(pack, name, path, width, height, D3DXVECTOR2(0.5f, 0.5f), offset);
}

void ImageManager::AddSprite(unordered_map<string, sprite*>& pack, string name, string path, float width, float height, D3DXVECTOR2 pivot, D3DXVECTOR2 offset)
{
	sprite* image = new sprite();
	image->width = width;
	image->height = height;
	image->pivot = pivot;
	image->offset = offset;
	if (D3DX11CreateShaderResourceViewFromFile(DxM.GetDevice(), path.c_str(), NULL, NULL, &image->texture, NULL) == E_FAIL)
	{
		name += " 저장에 문제가 생겼습니다.";
		MessageBox(DxM.GethWnd(), (LPCSTR)name.c_str(), "AddSprite", 0);
		return;
	}
	pack.insert(make_pair(name, image));
}
// 애니메이션용 이미지 애니메이션은 아래쪽에 중심을 잡는게 좋아보여서 만듬
void ImageManager::AddCharacterAnimationMaker(Animation * anime, string path, float width, float height, D3DXVECTOR2 pivot, D3DXVECTOR2 offset, float frame)
{
	sprite* image;
	if (_mCharacters.find(path) == _mCharacters.end())
	{
		image = new sprite();
		image->width = width;
		image->height = height;
		image->pivot = pivot;
		image->offset = offset;
		if (D3DX11CreateShaderResourceViewFromFile(DxM.GetDevice(), path.c_str(), NULL, NULL, &image->texture, NULL) == E_FAIL)
		{
			MessageBox(DxM.GethWnd(), (LPCSTR)path.c_str(), "AddAnimationFrame", 0);
			return;
		}
		_mCharacters.insert(make_pair(path, image));
	}
	else
	{
		image = _mCharacters[path];
	}
	anime->frame.push_back(frame);
	anime->texture.push_back(image);
}

void ImageManager::AddBackGroundAnimationMaker(Animation * anime, string path, float width, float height, D3DXVECTOR2 pivot, D3DXVECTOR2 offset, float frame)
{
	sprite* image;
	if (_mSceneSprites.find(path) == _mSceneSprites.end())
	{
		image = new sprite();
		image->width = width;
		image->height = height;
		image->pivot = pivot;
		image->offset = offset;
		if (D3DX11CreateShaderResourceViewFromFile(DxM.GetDevice(), path.c_str(), NULL, NULL, &image->texture, NULL) == E_FAIL)
		{
			MessageBox(DxM.GethWnd(), (LPCSTR)path.c_str(), "AddAnimationFrame", 0);
			return;
		}
		_mSceneSprites.insert(make_pair(path, image));
	}
	else
	{
		image = _mSceneSprites[path];
	}
	anime->frame.push_back(frame);
	anime->texture.push_back(image);
}

void ImageManager::LoadBelMyrePrincipality_South()
{

	Animation * anime;
	////////////////////////////
	//       엘븐가드		 //
	///////////////////////////

	AddSprite(_mSceneSprites, "back_far_0", "ImagePack\\sprite_map_town_04_elvengard_background\\back_far_0.png", 453, 279);
	AddSprite(_mSceneSprites, "back_far_1", "ImagePack\\sprite_map_town_04_elvengard_background\\back_far_1.png", 355, 279);
	AddSprite(_mSceneSprites, "back_far_2", "ImagePack\\sprite_map_town_04_elvengard_background\\back_far_2.png", 355, 279);

	AddSprite(_mSceneSprites, "back_middle_0", "ImagePack\\sprite_map_town_04_elvengard_background\\back_middle_0.png", 746, 340);
	//잔디 타일
	AddSprite(_mSceneSprites, "tile_0", "ImagePack\\sprite_map_town_04_elvengard_tile\\tile_0.png", 224, 276);
	AddSprite(_mSceneSprites, "tile_add40_0", "ImagePack\\sprite_map_town_04_elvengard_tile\\tile_add40_0.png", 224, 40);
	//대장간
	AddSprite(_mSceneSprites, "blacksmith_0", "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_0.png", 711, 358);
	//화로 내부 불 이미지
	anime = new Animation();
	anime->isLoop = true;
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_1.png", 64, 130, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0), 0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_2.png", 64, 130, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0), 0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_3.png", 64, 130, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0), 0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_4.png", 64, 130, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0), 0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_5.png", 64, 130, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0), 0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_6.png", 64, 130, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0), 0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_7.png", 64, 130, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0), 0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_8.png", 64, 130, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0), 0.1f);
	_mBackgroundGifs.insert(make_pair("blacksmith", anime));
	//화로 상단 TODO: 연기 작업 
	anime = new Animation();
	anime->isLoop = true;
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_smoke_0.png", 226, 292, D3DXVECTOR2(0.5f,0.5f),D3DXVECTOR2(0, 12),0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_smoke_1.png", 226, 316, D3DXVECTOR2(0.5f,0.5f),D3DXVECTOR2(0, 0), 0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_smoke_2.png", 195, 317, D3DXVECTOR2(0.5f,0.5f),D3DXVECTOR2(16, 0),0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_smoke_3.png", 195, 316, D3DXVECTOR2(0.5f,0.5f),D3DXVECTOR2(16, 0),0.1f); //offset 16, -40
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_smoke_4.png", 163, 237, D3DXVECTOR2(0.5f,0.5f),D3DXVECTOR2(4, 31),0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_smoke_5.png", 172, 254, D3DXVECTOR2(0.5f,0.5f),D3DXVECTOR2(0, 31),0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_smoke_6.png", 172, 245, D3DXVECTOR2(0.5f,0.5f),D3DXVECTOR2(0, 32),0.1f);
	AddBackGroundAnimationMaker(anime, "ImagePack\\sprite_map_town_04_elvengard_object\\blacksmith_smoke_7.png", 210, 267, D3DXVECTOR2(0.5f,0.5f),D3DXVECTOR2(-15, 33),0.1f);
	_mBackgroundGifs.insert(make_pair("blacksmith_smoke", anime));

	//바닥의 타일
	AddSprite(_mSceneSprites, "block_0", "ImagePack\\sprite_map_town_04_elvengard_object\\block_0.png", 353, 96);
	AddSprite(_mSceneSprites, "block_1", "ImagePack\\sprite_map_town_04_elvengard_object\\block_1.png", 247, 83);
	AddSprite(_mSceneSprites, "block_2", "ImagePack\\sprite_map_town_04_elvengard_object\\block_2.png", 262, 93);
	AddSprite(_mSceneSprites, "block_3", "ImagePack\\sprite_map_town_04_elvengard_object\\block_3.png", 141, 48);
	AddSprite(_mSceneSprites, "block_4", "ImagePack\\sprite_map_town_04_elvengard_object\\block_4.png", 181, 103);
	AddSprite(_mSceneSprites, "block_5", "ImagePack\\sprite_map_town_04_elvengard_object\\block_5.png", 190, 113);
	//
	AddSprite(_mSceneSprites, "myroom_0", "ImagePack\\sprite_map_town_04_elvengard_object\\myroom_0.png", 551, 366);
	AddSprite(_mSceneSprites, "tree_0", "ImagePack\\sprite_map_town_04_elvengard_object\\tree_0.png", 196, 385);
	AddSprite(_mSceneSprites, "tree_1", "ImagePack\\sprite_map_town_04_elvengard_object\\tree_1.png", 266, 381);
	AddSprite(_mSceneSprites, "tent_0", "ImagePack\\sprite_map_town_04_elvengard_object\\tent_0.png", 292, 158);
	AddSprite(_mSceneSprites, "seria_tree_n_0", "ImagePack\\sprite_map_town_04_elvengard_object\\seria_tree_n_0.png", 474, 395);
	AddSprite(_mSceneSprites, "sign_0", "ImagePack\\sprite_map_town_04_elvengard_object\\sign_0.png", 176, 268);
	AddSprite(_mSceneSprites, "sign_dungeon_0", "ImagePack\\sprite_map_town_04_elvengard_object\\sign_dungeon_0.png", 98, 194);

	AddSprite(_mSceneSprites, "near_0", "ImagePack\\sprite_map_town_04_elvengard_object\\near_0.png", 285, 105);
	AddSprite(_mSceneSprites, "near_1", "ImagePack\\sprite_map_town_04_elvengard_object\\near_1.png", 227, 92);

	// 다아아른 마을
}

void ImageManager::LoadPlayableCharacterPack()
{
	string path = "ImagePack\\sprite_character_gunner_atequipment_avatar_skin\\";
	string atlasname = "gg_body0000_";
	for (int i = 1; i < 5; i++)
	{
		string itos = std::to_string(i);
		string animatorName = atlasname + "p" + itos;
		string atlasnum = "_p" + itos;
		Animator controller = Animator();
		Animation * anime = new Animation();
		//마을 대기 모션
		anime->isLoop = true;
		AddCharacterAnimationMaker(anime, path + atlasname + "0" + atlasnum + ".png", 26, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "1" + atlasnum + ".png", 26, 126, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "2" + atlasnum + ".png", 26, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "3" + atlasnum + ".png", 26, 124, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "0" + atlasnum + ".png", 26, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "1" + atlasnum + ".png", 26, 126, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "2" + atlasnum + ".png", 26, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "3" + atlasnum + ".png", 26, 124, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "4" + atlasnum + ".png", 31, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "5" + atlasnum + ".png", 32, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "6" + atlasnum + ".png", 28, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "7" + atlasnum + ".png", 26, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "8" + atlasnum + ".png", 31, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(16, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "9" + atlasnum + ".png", 31, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(16, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "5" + atlasnum + ".png", 32, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		AddCharacterAnimationMaker(anime, path + atlasname + "4" + atlasnum + ".png", 31, 125, D3DXVECTOR2(1, 1), D3DXVECTOR2(15, 0), 0.3f);
		controller.AddClip("idle", anime);
		//박수..?
		/*anime = new Animation();
		AddAnimationFrame(anime, path + atlasname + "10" + atlasnum + ".png", 26, 126);
		AddAnimationFrame(anime, path + atlasname + "11" + atlasnum + ".png", 26, 126);
		AddAnimationFrame(anime, path + atlasname + "12" + atlasnum + ".png", 26, 126);
		AddAnimationFrame(anime, path + atlasname + "13" + atlasnum + ".png", 26, 126);
		controller->_mAnimations.insert(make_pair("", anime));*/

		//몬스터가 있을때 대기 >14->20->14 반복함
		anime = new Animation();
		anime->isLoop = true;
		AddCharacterAnimationMaker(anime, path + atlasname + "14" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "15" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "16" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "17" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "18" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "19" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "20" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "19" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "18" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "17" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "16" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "15" + atlasnum + ".png", 68, 123, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		controller.AddClip("battle_idle", anime);
		//기본 총 꺼내기
		anime = new Animation();
		AddCharacterAnimationMaker(anime, path + atlasname + "21" + atlasnum + ".png", 69, 118, D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.1f);
		AddCharacterAnimationMaker(anime, path + atlasname + "22" + atlasnum + ".png", 72, 120, D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.1f);
		AddCharacterAnimationMaker(anime, path + atlasname + "23" + atlasnum + ".png", 97, 120, D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.1f);
		AddCharacterAnimationMaker(anime, path + atlasname + "24" + atlasnum + ".png", 90, 122, D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.1f);
		controller.AddClip("first_defultAttack", anime);
		////총 반복
		anime = new Animation();
		AddCharacterAnimationMaker(anime, path + atlasname + "23" + atlasnum + ".png", 97, 120, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.2f);
		AddCharacterAnimationMaker(anime, path + atlasname + "24" + atlasnum + ".png", 90, 122, D3DXVECTOR2(0.5f, 1), D3DXVECTOR2(0, 0), 0.1f);
		controller.AddClip("repeat_defultAttack", anime);
		////3타 후 강한 총.
		//AddAnimationFrame(anime, path + atlasname + "25" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "26" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "27" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "28" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "29" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "30" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "31" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "32" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "33" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "34" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "35" + atlasnum + ".png", 26, 126);
		////게틀링건
		//AddAnimationFrame(anime, path + atlasname + "36" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "37" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "38" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "39" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "40" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "41" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "42" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "43" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "44" + atlasnum + ".png", 26, 126);
		////게틀링건 2(위쪽으로 발사)
		//AddAnimationFrame(anime, path + atlasname + "45" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "46" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "47" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "48" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "49" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "50" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "51" + atlasnum + ".png", 26, 126);

		////사방으로 총 쏘는 스킬 (안쓸거임)
		///*AddAnimationFrame(anime, path + atlasname + "52" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "53" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "54" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "55" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "56" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "57" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "58" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "59" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "60" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "61" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "62" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "63" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "64" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "65" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "66" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "67" + atlasnum + ".png", 26, 126);*/
		//걷기
		anime = new Animation();
		anime->isLoop = true;
		AddCharacterAnimationMaker(anime, path + atlasname + "68" + atlasnum + ".png", 25, 120,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "69" + atlasnum + ".png", 47, 120,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "70" + atlasnum + ".png", 68, 119,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "71" + atlasnum + ".png", 47, 116,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "72" + atlasnum + ".png", 23, 118,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "73" + atlasnum + ".png", 39, 117,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "74" + atlasnum + ".png", 65, 115,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "75" + atlasnum + ".png", 41, 119,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		controller.AddClip("walk", anime);
		//달리기
		anime = new Animation();
		anime->isLoop = true;
		AddCharacterAnimationMaker(anime, path + atlasname + "76" + atlasnum + ".png", 111, 76,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "77" + atlasnum + ".png", 110, 86,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "78" + atlasnum + ".png", 87, 82 ,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "79" + atlasnum + ".png", 113, 72,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,-20),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "80" + atlasnum + ".png", 110, 92,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		AddCharacterAnimationMaker(anime, path + atlasname + "81" + atlasnum + ".png", 87, 87 ,D3DXVECTOR2(0.5f,1),D3DXVECTOR2(0,0),0.13f);
		controller.AddClip("run", anime);
		////슬라이딩
		//AddAnimationFrame(anime, path + atlasname + "82" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "83" + atlasnum + ".png", 26, 126);
		////투척 무기
		///*AddAnimationFrame(anime, path + atlasname + "84" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "85" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "86" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "87" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "88" + atlasnum + ".png", 26, 126);*/
		////정면 피격모션
		//AddAnimationFrame(anime, path + atlasname + "89" + atlasnum + ".png", 26, 126);
		////후면 피격 모션
		//AddAnimationFrame(anime, path + atlasname + "90" + atlasnum + ".png", 26, 126);
		////다운
		//AddAnimationFrame(anime, path + atlasname + "91" + atlasnum + ".png", 26, 126);
		////퀵스탠딩 & 그냥 일어설때
		//AddAnimationFrame(anime, path + atlasname + "92" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "93" + atlasnum + ".png", 26, 126);
		////점프
		//AddAnimationFrame(anime, path + atlasname + "94" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "95" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "96" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "97" + atlasnum + ".png", 26, 126);
		////퀵스탠딩?
		//AddAnimationFrame(anime, path + atlasname + "98" + atlasnum + ".png", 26, 126);
		////점프 공격
		//AddAnimationFrame(anime, path + atlasname + "99" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "100" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "101" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "102" + atlasnum + ".png", 26, 126);
		////윈드밀
		//AddAnimationFrame(anime, path + atlasname + "103" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "104" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "105" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "106" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "107" + atlasnum + ".png", 26, 126);
		////니킥 비슷한거
		//AddAnimationFrame(anime, path + atlasname + "108" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "109" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "110" + atlasnum + ".png", 26, 126);
		////높이 차 올리는거
		//AddAnimationFrame(anime, path + atlasname + "111" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "112" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "113" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "114" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "115" + atlasnum + ".png", 26, 126);
		////덤블링?
		///*AddAnimationFrame(anime, path + atlasname + "116" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "117" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "118" + atlasnum + ".png", 26, 126);*/
		////앞방향으로 총구난사(안씀)
		///*AddAnimationFrame(anime, path + atlasname + "119" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "120" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "121" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "122" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "123" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "124" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "125" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "126" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "127" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "128" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "129" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "130" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "131" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "132" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "133" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "134" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "135" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "136" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "137" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "138" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "139" + atlasnum + ".png", 26, 126);*/
		////수류탄 던지기
		//AddAnimationFrame(anime, path + atlasname + "140" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "141" + atlasnum + ".png", 26, 126);
		//AddAnimationFrame(anime, path + atlasname + "142" + atlasnum + ".png", 26, 126);

		controller.AddNewParameta("bIdle", true);
		controller.AddNewParameta("fSpeed", 0.0f);
		//idle > walk
		Transition trans = Transition();
		trans.targetAnime = "walk";
		trans.AddCondition(controller.StringToHash("bIdle"), 0, 0, 0);
		trans.AddCondition(controller.StringToHash("fSpeed"), 2, 2, 0.49f);
		controller.MakeTransition("idle", trans);
		//idle > run
		trans = Transition();
		trans.targetAnime = "run";
		trans.AddCondition(controller.StringToHash("bIdle"), 0, 0, 0);
		trans.AddCondition(controller.StringToHash("fSpeed"), 2, 2, 0.9f);
		controller.MakeTransition("idle", trans);
		//walk > idle
		trans = Transition();
		trans.targetAnime = "idle";
		trans.AddCondition(controller.StringToHash("bIdle"), 0, 0, 1);
		controller.MakeTransition("walk", trans);
		//run > idle
		trans = Transition();
		trans.targetAnime = "idle";
		trans.AddCondition(controller.StringToHash("bIdle"), 0, 0, 1);
		controller.MakeTransition("run", trans);
		//run > walk
		trans = Transition();
		trans.targetAnime = "walk";
		trans.AddCondition(controller.StringToHash("fSpeed"), 2, 0, 0.9f);
		trans.AddCondition(controller.StringToHash("fSpeed"), 2, 2, 0.49f);
		controller.MakeTransition("run", trans);
		_mAnimatorControlls.insert(make_pair(animatorName, controller));
	}


}

void ImageManager::LoadSeriaRoom()
{
	AddSprite(_mSeriaSprites, "", "", 1, 1);
}

void ImageManager::LoadUI()
{
	AddSprite(_mUISprites, "SelectUnit", "Image\\SelectUnit.png", 100, 160, D3DXVECTOR2(0.5f,0.5f), D3DXVECTOR2(0, 0));
	//AddSprite(_mUISprites, "SelectUnit", "Image\\SelectUnit.png", 100, 130);
	//AddSprite(_mUISprites, "SelectUnit", "ImagePack\\sprite_map_town_04_elvengard_object\\myroom_0.png", 551, 366);
}
