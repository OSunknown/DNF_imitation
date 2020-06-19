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
	//생성된 케릭터(모델)에 대한 포인터 있어야됨
	//생성된 케릭터(모델)에 대한 명령 보낼 수 있어야됨
	//데미지 계산같은건 게임 매니저에다 보내서 생성하고
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

