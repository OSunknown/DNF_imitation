#include "stdafx.h"
#include "GameMaster.h"

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
