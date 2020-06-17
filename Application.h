#pragma once
#include "stdafx.h"
//==================================================================
//		## Application ## 
//==================================================================
/*
	�� ���α׷��� �߽��̿���

	InputManager ���� ���� ���� �ָ� SceneManager���� �ش�.

*/
class Application
{
public:
	Application();
	~Application();
	static Application& inse() { static Application instance; return instance; }
public:
	HRESULT Init(HINSTANCE hInstance);
	void Run();
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
private:
	HWND CreateWindows(HINSTANCE hInstance, int screenWidth, int screenHeight);
	void SetWindowSize(int width, int height);
private:
	HINSTANCE _hInstance;
	HWND _hWnd;
public:
	int DxscreenWidth;
	int DxscreenHeight;
	int windowPosX;
	int windowPosY;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

static Application * ApplicationHandle = NULL;