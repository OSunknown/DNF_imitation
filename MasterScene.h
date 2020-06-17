#pragma once
#include "Animator.h"
//==================================================================
//		## SceneManager ## (씬 매니져)
//==================================================================
/*
	TODO : 씬 매니저에서 카메라와 UI 에 관련된 데이터를 가지고 있었으면 하오.
	각 씬에선 움직일 수 없는 위치, NPC 등 그런걸 가지고 있게.

	그리는 순서는 뒷배경 > 바닥 >  케릭터 > 앞 배경 >UI 
	UI는 씬과 상관없이 그려야함 교체 화면은 UI쪽으로 넘기는게 좋을듯.
	InputManager로 부터 입력 받는 순서는 UI에 들어가서 반응하는 오브젝트가 없으면 false 로 반환되고
	이후 캐릭터> NPC > 바닥 순으로 체크

	UI순서 (던파에서)
	기본 UI > 퀘스트북 > 경매장 > 내정보> 장비 인벤토리> 모험단> 스킬

	입력받은 좌표가 오브젝트 안에 있는지 확인 하는 방법 생각
	1. 3d오브젝트를 2d 처럼 보이게 배치해서 레이를 쏳아 닿는 순서 대로 체크
	2. 오브젝트들에게 해당 좌표를 쏘아 자기 위에 있는지 체크 있으면 true 없으면 false
		true라면 체크 종료하고 이벤트발생
*/
//전방 선언
class Camera;
enum class sceneNames { None = 0, Scene_CharacterSelect, Scene_Elvenguard, };
class MasterScene
{
public:
	MasterScene();
	~MasterScene();
	static MasterScene & inse() { static MasterScene instance; return instance; }
public:
	HRESULT Init(void);
	void Shutdown(void);
	void Update(void);
	void FrameUpdate(void);
	HRESULT ChangeScene(sceneNames sceneNumber);
	void AddCharacter(int slotNum, D3DXVECTOR2 position);

public:
	bool sceneLoadOk = false;
	bool sceneUpdateLock = true;
	
	
private:
	sceneNames _currentScene;
	D3DXVECTOR3 _moveInput;
	vector<GameObject*> Background;
	vector<GameObject*> GameObjects;
	vector<GameObject*> Nearground;
	vector<GameObject*> UI;
	vector<Animator> animators;
	vector<SpriteObject*> backgrounds; //배경
	
	void LoadScene_CharacterSelect();
	void LoadScene_Elvenguard();
private:
	float Top();
	float Left();
	float Right();
	float Buttom();
	void SetScreenLimit(float top, float left, float right, float buttom);
	float _topLimit;
	float _LeftLimit;
	float _RightLimit;
	float _ButtomLimit;
};

