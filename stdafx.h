#pragma once

#define WIN32_LEAN_AND_MEAN			// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// Windows 헤더 파일:
#include <Windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>
// C++ 런타임 헤더 파일입니다.
#include <iostream>
// 자주 사용하는 STL
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
//==================================================================
//		## DxManager ## (다이렉트x 매니져)
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
Microsoft DirectX SDK (June 2010)\Include\D3D11.h(917,1) 와
Windows Kits\10\Include\10.0.18362.0\shared\winerror.h 매크로 정의 충돌남 시간나면 확인해서 수정할것.
*/
//==================================================================
//		## 네임 스페이스 ##
//==================================================================
using namespace std;
//==================================================================
//		## unordered_map에서 int key 값을 해쉬로 사용하는 방법 ##
//==================================================================
//출처 : http://veblush.blogspot.com/2012/10/map-vs-unorderedmap-for-string-key.html
struct bypass {
	size_t operator()(int v) const {
		return (size_t)v;
	}
};

//SOCKET listenSocket;
//==================================================================
//		## 내가 만든 헤더파일을 이곳에 추가한다 ##
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
//		## 싱글톤을 추가한다 ##
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
#include <locale.h> //한글로 바꿔주는 애
#include <thread>
#define MAX_BUFFER        4096
#define SERVER_IP        "192.168.0.15"
#define SERVER_PORT        11000

#define BOM8A 0xEF
#define BOM8B 0xBB
#define BOM8C 0xBF

/*
warning C4005: 매크로 재정의 문제는 아래 링크를 통해 해결하였다.
https://m.blog.naver.com/PostView.nhn?blogId=4roring&logNo=221203558526&proxyReferer=https:%2F%2Fwww.google.com%2F
*/