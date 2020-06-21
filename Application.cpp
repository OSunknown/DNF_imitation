#include "Application.h"



Application::Application()
{
	DxscreenWidth = 0;
	DxscreenHeight = 0;
	windowPosX = 0;
	windowPosY = 0;
	_hInstance = 0;
	_hWnd = 0;
}

Application::~Application()
{
}

HRESULT Application::Init(HINSTANCE hInstance)
{
	_hInstance = hInstance;
	ApplicationHandle = this;
	//옵션 파일 불러오는거 만들어서 데이터 집어넣기
	int screenWidth = 747;
	int screenHeight = 560;
	bool vsync = false;
	bool fullscreen = false;
	float screen_depth =100.0f;
	float screen_near = 0 ;

	CreateWindows(_hInstance, screenWidth, screenHeight);
	//4:3 한 화면당 최대 높이는 560 
	DxscreenWidth = 747;
	DxscreenHeight = 560;
	DxM.Init(DxscreenWidth, DxscreenHeight, vsync, _hWnd, fullscreen, screen_depth, screen_near);
	
	Network.Connect();
	Renderer.Init();
	IMAGEMANAGER.Init();
	Input.Init(hInstance, _hWnd, 747, 560);
	MainCamera.Init();
	Time.Init(300);
	//모두 설정한 뒤에 send를 해야됨
	Network.Send("1@DeusEx"); //user id
	return S_OK;
}

void Application::Run()
{
	MSG message;
	ZeroMemory(&message, sizeof(MSG));
	while (true)
	{
		if (MS.sceneUpdateLock == false)
		{
			break;
		}
	}
	MS.Init();
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		
		if (Input.IsKeyPressed(DIK_ESCAPE))
		{
			//나중에 UI Menu가 개발된다면 이쪽을 수정해서 활성화 시키자.
			MS.Shutdown();
			DxM.Shutdown();
			
			break;
		}
		else
		{
			Time.Update();
			//옵션이 변경되면 여기서 체크해서 적용되게 해주세요.
			//씬 넘어갈때 시간이 와르르 흐르는 문제가 있음. 2020-06-18
			//UIClear 함수로 교체 후 해결됨 
			//해당문제가 다시 발생할경우 MasterScene의 ChangeScene에서 Time.Update의 주석처리를 지울것. 2020-06-18
			if (Time.CheckFPSLimit()) 
			{
				Input.Frame();				
				DxM.BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
				DxM.EnableAlphaBlending();
				MainCamera.Render();
				MS.FrameUpdate();
				DxM.DisableAlphaBlending();
				DxM.EndScene();
				//Sleep(16.666666666666666666666666666667);//60fps?
			}
		}
	}
	return;
}

LRESULT Application::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg) {
	case WM_LBUTTONDOWN:
		//UI동작, 위치 이동.
		break;
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


//==================================================================
//		## 윈도우 창 생성 ##
//==================================================================
HWND Application::CreateWindows(HINSTANCE hInstance, int screenWidth, int screenHeight)
{
	LPCSTR m_applicationName = "DnF Capycat";
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
	
	windowPosX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	windowPosY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		windowPosX, windowPosY, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);
	ShowWindow(_hWnd, SW_SHOW);
	SetForegroundWindow(_hWnd);
	SetFocus(_hWnd);
	return _hWnd;
}
//==================================================================
//		## 윈도우 창 크기 ## (해상도 X)
//==================================================================
void Application::SetWindowSize(int width,int height)
{
	//width와 height는 새 창의 넓이(픽셀)
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos

	SetWindowPos(_hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE); //윈도우 창 크기 조절이에요
	DxM.SetWindowSize(width, height); //해상도 조절이에요
	return;
}

//==================================================================
//		## 윈도우 프로시져 ## (메시지를 운영체제에 전달)
//==================================================================
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return ApplicationHandle->MessageHandler(hwnd, msg, wparam, lparam);
}

