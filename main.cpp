#include "stdafx.h"
#include "Application.h"
#pragma warning(disable:4996) // AllocConsole ���ﶧ ���� ����
//==================================================================
//		## ������ �����Լ� ## 
//==================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//hInstance : ���α׷��� �ν��Ͻ� �ڵ�
	//hPrevInstance : ������ ����� �ν��Ͻ� �ڵ� (������ ������)
	//lpszCmdParam : ��������� �Էµ� ���α׷� �μ�
	//nCmdShow : ���α׷��� ���۵� ���� (�ּ�ȭ, ����ũ����..)
	
	if (AllocConsole())
	{
		freopen("CONOUT$", "w", stdout);
	}

	//Application* app = new Application();
	
	//�ʱ�ȭ�� �����ߴٸ� ����
	if (FAILED(App.Init(hInstance)))
	{
		return 0;
	}
	App.Run();

	
	return 0;
}