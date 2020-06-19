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
	_currentCharacterobject->SetUiType();
	_currentCharacterobject->SetPosition(D3DXVECTOR2(0,0));
	havePlayableCharacter = true;
}

void GameMaster::CharacterMove(D3DXVECTOR2 movepoint, bool isRun)
{
	//�ӽ� �������Դϴ�. DB ���� �޾ƿ;��� ���������� �ϴ� ���̷�.
	float dummyWalkXSpeed = 0.5f;
	float dummyRunXSpeed = 1.0f;
	float dummyYmoveSpeed = 0.5f;
	//

	D3DXVECTOR2 movePosition = D3DXVECTOR2(0,0);
	if (isRun == true)
	{
		movePosition.x = movepoint.x * dummyRunXSpeed + _currentCharacterobject->_position.x;
	}
	else
	{
		movePosition.x = movepoint.x * dummyWalkXSpeed + _currentCharacterobject->_position.x;
	}
	
	movePosition.y = movepoint.y * dummyYmoveSpeed + _currentCharacterobject->_position.y;
	_currentCharacterobject->SetPosition(movePosition);
}
