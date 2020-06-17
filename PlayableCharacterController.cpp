#include "stdafx.h"
#include "BaseCharacterObject.h"
#include "PlayableCharacterController.h"



PlayableCharacterController::PlayableCharacterController()
{
	_Filp = false;
	_isRun = false;
	_isMoveUP = false;
	_isMoveDown = false;
	_isMoveLeft = false;
	_isMoveRight = false;
	_moveInput = D3DXVECTOR2(0,0);
	command = vector<pair<string, float>>();
}


PlayableCharacterController::~PlayableCharacterController()
{
}

void PlayableCharacterController::FixedUpdate()
{
	// 눌렸다는건 움직이고 있다.
	PCController.InputMove(Input.IsKeyPressed(DIK_UP) || Input.IsKeyPressed(DIK_DOWN) || Input.IsKeyPressed(DIK_LEFT) || Input.IsKeyPressed(DIK_RIGHT));

	if (Input.IsKeyUp(DIK_UP))
	{
		_isMoveUP = false;

		if (_isMoveDown == false)
		{
			if (_isRun == true && (_isMoveLeft == false && _isMoveRight == false))
			{
				_isRun = false;
			}
			_moveInput.y = 0;
		}
	}

	if (Input.IsKeyUp(DIK_DOWN))
	{
		_isMoveDown = false;

		if (_isMoveUP == false)
		{
			if (_isRun == true && (_isMoveLeft == false && _isMoveRight == false))
			{
				_isRun = false;
			}
			_moveInput.y = 0;
		}
	}

	if (Input.IsKeyUp(DIK_LEFT))
	{
		_isMoveLeft = false;
		if (_Filp == true)
		{
			if (_isMoveUP == false && _isMoveDown == false)
			{
				if (_isRun == true)
				{
					_isRun = false;
				}
			}
			_moveInput.x = 0;
		}
	}

	if (Input.IsKeyUp(DIK_RIGHT))
	{
		_isMoveRight = false;
		if (_Filp == false)
		{
			if (_isMoveUP == false && _isMoveDown == false)
			{
				if (_isRun == true)
				{
					_isRun = false;
				}
				characterObject->Idle();				
			}
			_moveInput.x = 0;
		}
	}

	//움직일떄는 여기서
	if (Input.IsKeyDown(DIK_UP))
	{
		_isMoveUP = true;

		if (_isRun == false)
		{
			printf("MoveUpHold walk\n");
			characterObject->Walk();
		}
		//_moveInput.y = _data.walkSpeed;

		command.push_back(make_pair<string, float>("U", 0.8f));
	}

	if (Input.IsKeyDown(DIK_DOWN))
	{
		_isMoveDown = true;
		if (_isRun == false)
		{
			printf("walk\n");
			characterObject->Walk();
		}
		//_moveInput.y = -_data.walkSpeed;
		command.push_back(make_pair<string, float>("D", 0.8f));
	}

	if (Input.IsKeyDown(DIK_LEFT))
	{
		if (_Filp == false)
		{
			_isRun = false;
		}
		_Filp = true;
		_isMoveLeft = true;
		if ((!command.empty() && command.back().first == "L") || _isRun == true)
		{
			_isRun = true;
			printf("run\n");
			characterObject->Run();
			//_moveInput.x = -_data.runSpeed;
		}

		if (_isRun == false)
		{
			printf("walk\n");
			characterObject->Walk();
			//_moveInput.x = -_data.walkSpeed;
		}

		command.push_back(make_pair<string, float>("L", 0.8f));
	}

	if (Input.IsKeyDown(DIK_RIGHT))
	{
		if (_Filp == true)
		{
			_isRun = false;
		}
		_Filp = false;
		_isMoveRight = true;
		if ((!command.empty() && command.back().first == "R") || _isRun == true)
		{
			_isRun = true;
			printf("run\n");
			characterObject->Run();
			//_moveInput.x = _data.runSpeed;
		}

		if (_isRun == false)
		{
			printf("walk\n");
			characterObject->Walk();
			//_moveInput.x = _data.walkSpeed;
		}

		command.push_back(make_pair<string, float>("R", 0.8f));
	}

	if (Input.IsKeyDown(DIK_X))
	{
		command.push_back(make_pair<string, float>("X", 0.8f));
	}

	if (Input.IsKeyDown(DIK_Z))
	{
		command.push_back(make_pair<string, float>("Z", 0.8f));
	}

	if (Input.IsKeyDown(DIK_C))
	{
		command.push_back(make_pair<string, float>("C", 0.8f));
	}

	//MainCamera.SetPostion(Player.GetPosition());
}

void PlayableCharacterController::InputMove(bool bMove)
{
	_isMove = bMove;
}
