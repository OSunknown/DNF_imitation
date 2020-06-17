#include "stdafx.h"
#include "Application.h"
#pragma warning(disable:4996) // AllocConsole 지울때 같이 삭제
//==================================================================
//		## 윈도우 메인함수 ## 
//==================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//hInstance : 프로그램의 인스턴스 핸들
	//hPrevInstance : 이전에 실행된 인스턴스 핸들 (지금은 사용안함)
	//lpszCmdParam : 명령행으로 입력된 프로그램 인수
	//nCmdShow : 프로그램이 시작될 형태 (최소화, 보통크기등등..)
	
	if (AllocConsole())
	{
		freopen("CONOUT$", "w", stdout);
	}

	//Application* app = new Application();
	
	//초기화를 실패했다면 종료
	if (FAILED(App.Init(hInstance)))
	{
		return 0;
	}
	App.Run();

	
	return 0;
}