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
