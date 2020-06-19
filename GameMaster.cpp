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
		//배열로 저장한다.
		//필요 데이터는 순서에서 값을 가져온다.
		//가독성을 위해 반환값에는 enum 을 이용하여
		//텍스트로써 보이게한다.
		detail = strtok_s(NULL, ";", &data);
		if (detail == nullptr)break;
		cdata.CharacterDatas.push_back(detail);
		printf("%s", detail);
		/*한글인지 체크하시오.
		출처:https://wondangcom.com/m/510?category=717981
		내 게임에선 특수문자는 안되.특수문자 구분은 제외한다.
		if (detail[0] & 0x80) //한글 출력 부분을 구현 못해서 넘김.
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
	//임시 데이터입니다. DB 에서 받아와야할 데이터지만 일단 더미로.
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
