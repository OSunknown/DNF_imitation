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
	//�ɼ� ���� �ҷ����°� ���� ������ ����ֱ�
	int screenWidth = 747;
	int screenHeight = 560;
	bool vsync = false;
	bool fullscreen = false;
	float screen_depth =100.0f;
	float screen_near = 0 ;

	CreateWindows(_hInstance, screenWidth, screenHeight);
	//4:3 �� ȭ��� �ִ� ���̴� 560 
	DxscreenWidth = 747;
	DxscreenHeight = 560;
	DxM.Init(DxscreenWidth, DxscreenHeight, vsync, _hWnd, fullscreen, screen_depth, screen_near);
	
	Network.Connect();
	Renderer.Init();
	IMAGEMANAGER.Init();
	Input.Init(hInstance, _hWnd, 747, 560);
	MainCamera.Init();
	Time.Init(60);
	//��� ������ �ڿ� send�� �ؾߵ�
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
			//���߿� UI Menu�� ���ߵȴٸ� ������ �����ؼ� Ȱ��ȭ ��Ű��.
			MS.Shutdown();
			DxM.Shutdown();
			
			break;
		}
		else
		{
			Time.Update();
			//�ɼ��� ����Ǹ� ���⼭ üũ�ؼ� ����ǰ� ���ּ���.
			if (Time.CheckFPSLimit())
			{
				Input.Frame();				
				DxM.BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
				DxM.EnableAlphaBlending();
				MainCamera.Render();
				MS.FrameUpdate();
				DxM.DisableAlphaBlending();
				DxM.EndScene();
			}
		}
	}
	return;
}

LRESULT Application::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg) {
	case WM_LBUTTONDOWN:
		//UI����, ��ġ �̵�.
		break;
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


//==================================================================
//		## ������ â ���� ##
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
//		## ������ â ũ�� ## (�ػ� X)
//==================================================================
void Application::SetWindowSize(int width,int height)
{
	//width�� height�� �� â�� ����(�ȼ�)
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos

	SetWindowPos(_hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE); //������ â ũ�� �����̿���
	DxM.SetWindowSize(width, height); //�ػ� �����̿���
	return;
}

//==================================================================
//		## ������ ���ν��� ## (�޽����� �ü���� ����)
//==================================================================
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return ApplicationHandle->MessageHandler(hwnd, msg, wparam, lparam);
}

