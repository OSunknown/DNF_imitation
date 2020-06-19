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
	if (GM.havePlayableCharacter == false)
		return;//케릭터가 생성되기 전이라 되돌아가라.

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
				GM.GetCurrentCharacterObject()->Idle();
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
				cout << "R > Idle" << endl;
				GM.GetCurrentCharacterObject()->Idle();				
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
			GM.GetCurrentCharacterObject()->Walk();
		}
		_moveInput.y = 1.0f;
		command.push_back(make_pair<string, float>("U", 0.8f));
	}

	if (Input.IsKeyDown(DIK_DOWN))
	{
		_isMoveDown = true;
		if (_isRun == false)
		{
			cout << "Dwalk" << endl;
			GM.GetCurrentCharacterObject()->Walk();
		}
		_moveInput.y = -1.0f;
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
			cout << "Lrun" << endl;
			GM.GetCurrentCharacterObject()->Run();
		}

		if (_isRun == false)
		{
			cout << "Lwalk" << endl;
			GM.GetCurrentCharacterObject()->Walk();
			
		}
		_moveInput.x = -1.0f;
		command.push_back(make_pair<string, float>("L", 0.8f));
	}

	if (Input.IsKeyDown(DIK_RIGHT))
	{
		cout << "DIK_RIGHT" << endl;
		if (_Filp == true)
		{
			_isRun = false;
		}
		_Filp = false;
		_isMoveRight = true;
		if ((!command.empty() && command.back().first == "R") || _isRun == true)
		{
			_isRun = true;
			cout << "Rrun" << endl;
			GM.GetCurrentCharacterObject()->Run();
		}

		if (_isRun == false)
		{
			cout << "Rwalk" << endl;
			GM.GetCurrentCharacterObject()->Walk();
			
		}
		_moveInput.x = 1.0f;
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

	GM.CharacterMove(_moveInput, _isRun);

	//시간이 지난 커맨드 키는 날려준다.
	vector<pair<string, float>> tempCommand;
	for (int i = 0; i < command.size(); i++)
	{
		command[i].second -= Time.GetDeltaTime();
		if (command[i].second > 0.0f)
		{
			tempCommand.push_back(command[i]);
		}
	}
	command = tempCommand;

	//커맨드 확인
	int selectSkillNumber = -1;
	for (int i = 0; i < command.size(); i++)
	{
		string commandCheck = "";
		//한칸씩 떙겨가며 커맨드를 확인해야되.
		for (int j = i; j < command.size(); j++)
		{
			if (_Filp == false)
			{
				//R 이 Front L이 Back
				if (command[j].first == "R")
				{
					commandCheck += "F";
				}
				else if (command[j].first == "L")
				{
					commandCheck += "B";
				}
				else
				{
					commandCheck += command[j].first;
				}
			}
			else
			{
				//L 이 Front R이 Back
				if (command[j].first == "R")
				{
					commandCheck += "B";
				}
				else if (command[j].first == "L")
				{
					commandCheck += "F";
				}
				else
				{
					commandCheck += command[j].first;
				}
			}
		}
		//디버그용 커맨드가 입력이 이상하다 싶으면 풀어서 사용		
		if (command[command.size() - 1].first == "X" || command[command.size() - 1].first == "Z" || command[command.size() - 1].first == "S")
		{
			int t = 1;
		}
		//커맨드를 확인하고, 있다면 break 를 걸어서 밖으로 빠져나가야된다.
		//스킬들은 string, int 로 저장해서 커맨드를 string에 저장하고 스킬 번호인 int를 
		//반환한다.

		//TODO: 스킬 추가할 차례야.
		if (commandCheck == "BFBFX")
		{
			int T = 1;
		}

		if (commandCheck == "BUFX")
		{
			int T = 1;
		}
		if (commandCheck == "FDDX")
		{
			int T = 1;
		}

		if (commandCheck == "Z")
		{
			selectSkillNumber = 1;
			break;
		}
	}
	//스킬번호가 -1이 아니면 커맨드를 한번 비워주자. 어짜피 연결되지않는다.
	if (selectSkillNumber != -1)
	{
		//스킬이 입력되지 않았다면 슬라이딩을 쓸 수 있어야한다.
		if (command[command.size() - 1].first == "X")
		{
			if (_isRun == true)
			{
				//슬라이딩
			}
			else
			{
				//일반 공격
			}
		}
		command.clear();
	}
	//스킬 번호를 switch 값으로 처리해서 각 함수를 호출하자.
	switch (selectSkillNumber)
	{

	}
}

void PlayableCharacterController::InputMove(bool bMove)
{
	_isMove = bMove;
}
