#pragma once
#include "Animator.h"
//==================================================================
//		## SceneManager ## (�� �Ŵ���)
//==================================================================
/*
	TODO : �� �Ŵ������� ī�޶�� UI �� ���õ� �����͸� ������ �־����� �Ͽ�.
	�� ������ ������ �� ���� ��ġ, NPC �� �׷��� ������ �ְ�.

	�׸��� ������ �޹�� > �ٴ� >  �ɸ��� > �� ��� >UI 
	UI�� ���� ������� �׷����� ��ü ȭ���� UI������ �ѱ�°� ������.
	InputManager�� ���� �Է� �޴� ������ UI�� ���� �����ϴ� ������Ʈ�� ������ false �� ��ȯ�ǰ�
	���� ĳ����> NPC > �ٴ� ������ üũ

	UI���� (���Ŀ���)
	�⺻ UI > ����Ʈ�� > ����� > ������> ��� �κ��丮> �����> ��ų

	�Է¹��� ��ǥ�� ������Ʈ �ȿ� �ִ��� Ȯ�� �ϴ� ��� ����
	1. 3d������Ʈ�� 2d ó�� ���̰� ��ġ�ؼ� ���̸� �R�� ��� ���� ��� üũ
	2. ������Ʈ�鿡�� �ش� ��ǥ�� ��� �ڱ� ���� �ִ��� üũ ������ true ������ false
		true��� üũ �����ϰ� �̺�Ʈ�߻�
*/
//���� ����
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
	void AddUIObject(GameObject* ui);
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
	vector<SpriteObject*> backgrounds; //���
	
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

