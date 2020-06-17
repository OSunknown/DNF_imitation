#pragma once

////==================================================================
////		## DxManager ## (다이렉트x 매니져)
////==================================================================
//// Linking
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "d3dx10.lib")
//
//// Includes
//#include <DXGI.h>
//#include <D3Dcommon.h>
//#include <D3D11.h>

class DxManager
{
public:
	DxManager();
	~DxManager();
	static DxManager & inse() { static DxManager instance; return instance; }
public:
	HRESULT Init(int screenWidth, int screenHeight, bool bVsync, HWND hWnd, bool bFullscreen, float screenDepth, float screenNear);
	void VsyncChecker(int screenWidth, int screenHeight);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice() { return _device; };
	ID3D11DeviceContext* GetDeviceContext() { return _deviceContext; };
	HWND GethWnd();
	void GetProjectionMatrix(D3DXMATRIX& projectionMatrix) { projectionMatrix = _projectionMatrix;};
	void GetWorldMatrix(D3DXMATRIX& worldMatrix) { worldMatrix = _worldMatrix;};
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix) { orthoMatrix = _orthoMatrix;};

	void GetVideoCardInfo(char*, int&);

	D3DXVECTOR2 GetScreenSize();

	void TurnZBufferOn();
	void TurnZBufferOff();
	void EnableAlphaBlending();
	void DisableAlphaBlending();


	void SetWindowSize(int width, int height);
private:

	HWND _hWnd;
	D3DXVECTOR2 _screenSize;

	bool _vsync_enabled;
	unsigned int numerator;
	unsigned int denominator;

	int _videoCardMemory;
	char _videoCardDescription[128];
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11RasterizerState* _rasterState;
	D3DXMATRIX _projectionMatrix;
	D3DXMATRIX _worldMatrix;
	D3DXMATRIX _orthoMatrix;
	ID3D11DepthStencilState* _depthDisabledStencilState;
	ID3D11BlendState* _alphaEnabledBlendState;
	ID3D11BlendState* _alphaDisabledBlendState;
};

