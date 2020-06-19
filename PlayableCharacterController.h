#pragma once

class BaseCharacterObject;
class PlayableCharacterController
{
public:
	PlayableCharacterController();
	~PlayableCharacterController();
	static PlayableCharacterController & inse() { static PlayableCharacterController instance; return instance; }
public:
	void Init();

	void FixedUpdate();
	
	void InputMove(bool bMove);
public:
	//������ �ɸ���(��)�� ���� ������ �־�ߵ�
	//������ �ɸ���(��)�� ���� ��� ���� �� �־�ߵ�
	//������ ��갰���� ���� �Ŵ������� ������ �����ϰ�
	//BaseCharacterObject * characterObject;
public:
	bool _Filp;
	bool _isRun;
	bool _isMoveUP;
	bool _isMoveDown;
	bool _isMoveLeft;
	bool _isMoveRight;
	D3DXVECTOR2 _moveInput;
	vector<pair<string, float>> command;

	bool _isMove;
};

