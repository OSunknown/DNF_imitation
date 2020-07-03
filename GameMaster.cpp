#include "stdafx.h"
#include "GameMaster.h"
#include "BaseCharacterObject.h"
GameMaster::GameMaster()
{
}


GameMaster::~GameMaster()
{
}

void GameMaster::Init()
{

}

int GameMaster::SetCharacterData(char* data)
{
	CharacterData cdata;
	char* detail;
	int i = 0;
	while (true)
	{
		//�迭�� �����Ѵ�.
		//�ʿ� �����ʹ� �������� ���� �����´�.
		//�������� ���� ��ȯ������ enum �� �̿��Ͽ�
		//�ؽ�Ʈ�ν� ���̰��Ѵ�.
		detail = strtok_s(NULL, ";", &data);
		if (detail == nullptr)break;
		cdata.CharacterDatas.push_back(detail);
		printf("%s", detail);
		/*�ѱ����� üũ�Ͻÿ�.
		��ó:https://wondangcom.com/m/510?category=717981
		�� ���ӿ��� Ư�����ڴ� �ȵ�.Ư������ ������ �����Ѵ�.
		if (detail[0] & 0x80) //�ѱ� ��� �κ��� ���� ���ؼ� �ѱ�.
		{ 
			wprintf(L"%d %s  :Type2\n", i, UTF8_to_WChar(detail));
			//then korean,  isEnglishName = false;
			for (int j = 0; ; j++)
			{
				if (detail[j] == '\0')
					break;
				putchar(detail[j]);
			}
		}
		else
		{
			printf("%d %s  :Type1\n", i, detail);
		}
		i++;*/
	}slots.push_back(cdata);
	return i;
}

BaseCharacterObject* GameMaster::GetCurrentCharacterObject()
{
	return _currentCharacterobject;
}

void GameMaster::SetCurrentCharacterObject(int slots)
{
	_currentCharacterobject = new BaseCharacterObject();
	_currentCharacterobject->init(slots);
	_currentCharacterobject->SetPosition(D3DXVECTOR2(0,0));
	havePlayableCharacter = true;
}

void GameMaster::CharacterMove(D3DXVECTOR2 movepoint, bool isRun)
{
	//�ӽ� �������Դϴ�. DB ���� �޾ƿ;��� ���������� �ϴ� ���̷�.
	float dummyWalkXSpeed = 100.0f * Time.GetDeltaTime();
	float dummyRunXSpeed = 150.0f * Time.GetDeltaTime();
	float dummyYmoveSpeed = 50.0f * Time.GetDeltaTime();
	//

	D3DXVECTOR2 movePosition = D3DXVECTOR2(0,0);
	if (isRun == true)
	{
		movePosition.x = movepoint.x * dummyRunXSpeed  ;
	}
	else
	{
		movePosition.x = movepoint.x * dummyWalkXSpeed;
	}
	
	movePosition.y = movepoint.y * dummyYmoveSpeed ;

	//Master Scene�� �����ؼ� ���� �ִ� ��ġ���� üũ.
	
	_currentCharacterobject->SetPosition(MS.moveAreaCheck(movePosition, _currentCharacterobject->_position));
}

void GameMaster::CharacterFilp(bool Filp)
{
	_currentCharacterobject->SetFlip(Filp);
}
