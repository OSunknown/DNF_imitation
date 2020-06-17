#pragma once

#define WIN32_LEAN_AND_MEAN			// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

// Windows ��� ����:
#include <Windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>
// C++ ��Ÿ�� ��� �����Դϴ�.
#include <iostream>
// ���� ����ϴ� STL
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
//==================================================================
//		## DxManager ## (���̷�Ʈx �Ŵ���)
//==================================================================
// Linking
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

// Includes
#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>
#include <D3DX10math.h>
/* TODO:
Microsoft DirectX SDK (June 2010)\Include\D3D11.h(917,1) ��
Windows Kits\10\Include\10.0.18362.0\shared\winerror.h ��ũ�� ���� �浹�� �ð����� Ȯ���ؼ� �����Ұ�.
*/
//==================================================================
//		## ���� �����̽� ##
//==================================================================
using namespace std;
//==================================================================
//		## unordered_map���� int key ���� �ؽ��� ����ϴ� ��� ##
//==================================================================
//��ó : http://veblush.blogspot.com/2012/10/map-vs-unorderedmap-for-string-key.html
struct bypass {
	size_t operator()(int v) const {
		return (size_t)v;
	}
};

//SOCKET listenSocket;
//==================================================================
//		## ���� ���� ��������� �̰��� �߰��Ѵ� ##
//==================================================================
#include "Application.h"
#include "DxManager.h"
#include "RenderManager.h"
#include "NetworkManager.h"
#include "Camera.h"
#include "ImageManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SpriteObject.h"
#include "ImageObject.h"
#include "GameObject.h"
#include "PlayableCharacterController.h"

#include "Animator.h"
#include "MasterScene.h"
#include "GameMaster.h"
//==================================================================
//		## �̱����� �߰��Ѵ� ##
//==================================================================
#define App Application::inse()
#define DxM DxManager::inse()
#define Renderer RenderManager::inse()
#define GM GameMaster::inse()
#define MS MasterScene::inse()
#define MainCamera Camera::inse()
#define IMAGEMANAGER ImageManager::inse()
#define Time TimeManager::inse()
#define Input InputManager::inse()
#define PCController PlayableCharacterController::inse()
#define Network NetworkManager::inse()
//==================================================================
//		## Network ##
//==================================================================
#pragma comment(lib,"Ws2_32.lib")
#include <locale.h> //�ѱ۷� �ٲ��ִ� ��
#include <thread>
#define MAX_BUFFER        4096
#define SERVER_IP        "192.168.0.15"
#define SERVER_PORT        11000

#define BOM8A 0xEF
#define BOM8B 0xBB
#define BOM8C 0xBF

/*
warning C4005: ��ũ�� ������ ������ �Ʒ� ��ũ�� ���� �ذ��Ͽ���.
https://m.blog.naver.com/PostView.nhn?blogId=4roring&logNo=221203558526&proxyReferer=https:%2F%2Fwww.google.com%2F
*/