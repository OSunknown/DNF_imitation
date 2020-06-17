#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
class InputManager
{
public:
	InputManager();
	~InputManager();
	static InputManager & inse() { static InputManager insetance; return insetance; };
public:
	bool Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Frame();
	void Shutdown();

	bool IsKeyPressed(int key);
	bool IsKeyDown(int key);
	bool IsKeyDownWaitforSeconds(int key,float time);
	bool IsKeyUp(int key);

	bool GetMouseButtonDown(int mousebuttonNumber);

	void GetMouseLocation(int&, int&);
	void ScreenToWorldPoint(int&, int&);
private:
	void ReadKeyboard();
	void ReadMouse();
	void ProcessInput();
private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	unsigned char _keyboardState[256];
	unsigned char _keyboardkeyUpDown[256];
	bool _keyboardkeyDown[256];
	bool _keyboardkeyUp[256];

	float _keyboardWaitforScened[256];

	DIMOUSESTATE _mouseState;

	int _screenWidth, _screenHeight;
	int _mouseX, _mouseY;
};

