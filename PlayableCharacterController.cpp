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
		return;//�ɸ��Ͱ� �����Ǳ� ���̶� �ǵ��ư���.

	// ���ȴٴ°� �����̰� �ִ�.
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

	//�����ϋ��� ���⼭
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

	//�ð��� ���� Ŀ�ǵ� Ű�� �����ش�.
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

	//Ŀ�ǵ� Ȯ��
	int selectSkillNumber = -1;
	for (int i = 0; i < command.size(); i++)
	{
		string commandCheck = "";
		//��ĭ�� ���ܰ��� Ŀ�ǵ带 Ȯ���ؾߵ�.
		for (int j = i; j < command.size(); j++)
		{
			if (_Filp == false)
			{
				//R �� Front L�� Back
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
				//L �� Front R�� Back
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
		//����׿� Ŀ�ǵ尡 �Է��� �̻��ϴ� ������ Ǯ� ���		
		if (command[command.size() - 1].first == "X" || command[command.size() - 1].first == "Z" || command[command.size() - 1].first == "S")
		{
			int t = 1;
		}
		//Ŀ�ǵ带 Ȯ���ϰ�, �ִٸ� break �� �ɾ ������ ���������ߵȴ�.
		//��ų���� string, int �� �����ؼ� Ŀ�ǵ带 string�� �����ϰ� ��ų ��ȣ�� int�� 
		//��ȯ�Ѵ�.

		//TODO: ��ų �߰��� ���ʾ�.
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
	//��ų��ȣ�� -1�� �ƴϸ� Ŀ�ǵ带 �ѹ� �������. ��¥�� ��������ʴ´�.
	if (selectSkillNumber != -1)
	{
		//��ų�� �Էµ��� �ʾҴٸ� �����̵��� �� �� �־���Ѵ�.
		if (command[command.size() - 1].first == "X")
		{
			if (_isRun == true)
			{
				//�����̵�
			}
			else
			{
				//�Ϲ� ����
			}
		}
		command.clear();
	}
	//��ų ��ȣ�� switch ������ ó���ؼ� �� �Լ��� ȣ������.
	switch (selectSkillNumber)
	{

	}
}

void PlayableCharacterController::InputMove(bool bMove)
{
	_isMove = bMove;
}
