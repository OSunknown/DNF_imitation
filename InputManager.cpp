#include "stdafx.h"
#include "InputManager.h"
#include <windows.h>


InputManager::InputManager()
{
	_mouseState = DIMOUSESTATE();
	_keyboardState[0] = { '\0', };
	_keyboardkeyUpDown[0] = { '\0', };
	_keyboardkeyDown[0] = { '\0', };
	_keyboardkeyUp[0] = { '\0', };
	_keyboardWaitforScened[0] = { '\0', };

	_directInput = 0;
	_keyboard = 0;
	_mouse = 0;
	_screenWidth = 0;
	_screenHeight = 0;
	_mouseX = 0;
	_mouseY = 0;

}


InputManager::~InputManager()
{
}

bool InputManager::Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_mouseX = 0;
	_mouseY = 0;

	DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInput, NULL);

	// Ű���� �Է� ��ġ �ʱ�ȭ
	_directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);

	_keyboard->SetDataFormat(&c_dfDIKeyboard);

	// Ű���� ���� ������ ���� ���·� ����
	_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	// �� �����ͷ� Ű���忡 ���� ������ ���
	_keyboard->Acquire();

	// ���콺 �Է� ��ġ �ʱ�ȭ 
	_directInput->CreateDevice(GUID_SysMouse, &_mouse, NULL);

	_mouse->SetDataFormat(&c_dfDIMouse);

	// ���콺 ���� ������ ����� ���·� ����
	_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	// �� �����ͷ� Ű���忡 ���� ������ ���
	_mouse->Acquire();


	return true;
}

void InputManager::Frame()
{
	ReadKeyboard();
	ReadMouse();
	ProcessInput();

	for (int i = 0; i < 256; i++)
	{
		if (_keyboardWaitforScened[i] != 0)
		{
			_keyboardWaitforScened[i] -= Time.GetDeltaTime();
			if (_keyboardWaitforScened[i] < 0.0)
			{
				_keyboardWaitforScened[i] = 0;
			}
		}
	}
}

void InputManager::Shutdown()
{
	if (_mouse)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = 0;
	}

	if (_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = 0;
	}

	if (_directInput)
	{
		_directInput->Release();
		_directInput = 0;
	}
}

bool InputManager::IsKeyPressed(int key)
{
	if (_keyboardState[key] & 0x80) // And ����
	{
		return true;
	}
	return false;
}

bool InputManager::IsKeyDown(int key)
{
	return _keyboardkeyDown[key];
}

bool InputManager::IsKeyDownWaitforSeconds(int key, float time)
{

	if (_keyboardWaitforScened[key] != 0)
		return false;

	if (_keyboardState[key] & 0x80) // And ����
	{
		_keyboardWaitforScened[key] = time;
		return true;
	}

	return false;
}

bool InputManager::IsKeyUp(int key)
{
	return _keyboardkeyUp[key];
}

bool InputManager::GetMouseButtonDown(int mousebuttonNumber)
{
	if (_mouseState.rgbButtons[mousebuttonNumber] & 0x80)
	{
		return true;
	}
	return false;
}

void InputManager::GetMouseLocation(int &mouseX, int &mouseY)
{
	mouseX = _mouseX;
	mouseY = _mouseY;
}

void InputManager::ScreenToWorldPoint(int &mouseX, int &mouseY)
{
	mouseX = _mouseX + (int)MainCamera.GetPosition().x - (App.DxscreenWidth / 2);
	mouseY = _mouseY + (int)MainCamera.GetPosition().y - (App.DxscreenHeight / 2);
}

void InputManager::ReadKeyboard()
{
	HRESULT result;

	result = _keyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_keyboard->Acquire();
		}
		else
		{
			assert(false);
		}
	}

	for (int key = 0; key < 256; key++)
	{
		_keyboardkeyDown[key] = false;
		_keyboardkeyUp[key] = false;
		if ((_keyboardState[key] & 0x80) && (_keyboardState[key] ^ _keyboardkeyUpDown[key]))
		{
			_keyboardkeyUpDown[key] = _keyboardState[key];
			_keyboardkeyDown[key] = true; 
		}
		if ((_keyboardState[key] == 0) && (_keyboardState[key] ^ _keyboardkeyUpDown[key]))
		{
			_keyboardkeyUpDown[key] = _keyboardState[key];
			_keyboardkeyUp[key] = true;
		}

	}
}

void InputManager::ReadMouse()
{
	HRESULT result;

	result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_mouse->Acquire();
		}
		else
		{
			assert(false);
		}
	}
}

void InputManager::ProcessInput()
{
	POINT p;
	if (GetCursorPos(&p))
	{
		//�������� ��ġ�� ���� ��ǥ�� 0,0���� �����ش�.
		_mouseX = (p.x - App.windowPosX);
		//���� ���� y�� 0�̹Ƿ� ��ũ�� ���̸� �̿��� �������ش�.
		_mouseY  = App.DxscreenHeight - (p.y - App.windowPosY); 

		if (_mouseX < 0) { _mouseX = 0; }
		if (_mouseY < 0) { _mouseY = 0; }

		if (_mouseX > _screenWidth) { _mouseX = _screenWidth; }
		if (_mouseY > _screenHeight) { _mouseY = _screenHeight; }

		
	}
	

	/*_mouseX += _mouseState.lX;
	_mouseY += _mouseState.lY;
	
	if (_mouseX < 0) { _mouseX = 0; }
	if (_mouseY < 0) { _mouseY = 0; }

	if (_mouseX > _screenWidth) { _mouseX = _screenWidth; }
	if (_mouseY > _screenHeight) { _mouseY = _screenHeight; }*/
}
