#include "stdafx.h"
#include "DxManager.h"



DxManager::DxManager()
{
	_hWnd = NULL;
	_screenSize = D3DXVECTOR2(0,0);

	_vsync_enabled  = false;
	numerator = 0;
	denominator = 0;

	_videoCardMemory = 0;
	_videoCardDescription[0] = { '\0', };

	_swapChain = NULL;
	_device = NULL;
	_deviceContext = NULL;
	_renderTargetView = NULL;
	_depthStencilBuffer = NULL;
	_depthStencilState = NULL;
	_depthStencilView = NULL;
	_rasterState = NULL;
	/*D3DXMATRIX _projectionMatrix;
	D3DXMATRIX _worldMatrix;
	D3DXMATRIX _orthoMatrix;*/
	_depthDisabledStencilState = NULL;
	_alphaEnabledBlendState = NULL;
	_alphaDisabledBlendState = NULL;
}


DxManager::~DxManager()
{
}

HRESULT DxManager::Init(int screenWidth, int screenHeight, bool bVsync, HWND hWnd, bool bFullscreen, float screenDepth, float screenNear)
{
	_hWnd = hWnd;
	_screenSize = D3DXVECTOR2((float)screenWidth, (float) screenHeight);
	// 수직동기화 
	if (bVsync)
	{
		VsyncChecker(screenWidth, screenHeight);
		if (_vsync_enabled == false)
		{
			if (MessageBox(hWnd,
				"모니터가 해당 해상도를 지원하지 않아여 vsync없이 계속 진행하쉴?",
				"Vsync Failure", MB_YESNO) == IDNO)
			{
				// TODO:게임 종료 구현해 주세여.
			}
		}
	}

	//========================[DXGI_SWAP_CHAIN_DESC]================================
	//	.BufferCount : 백 버퍼의 갯수.(이중버퍼링 : 1개, 삼중버퍼링 : 2개)
	//		1. [1번 버퍼] -> 화면에 뿌림. [2번 버퍼] -> 그림
	//		2. [1번 버퍼] <- 교체 -> [2번 버퍼]
	//		3. [2번 버퍼] -> 화면에 뿌림. [1번 버퍼] -> 그림
	//
	//	[BufferDesc] : 생성하고자 하는 Back buffer의 속성들을 서술하는 구조체
	//		참고: https://msdn.microsoft.com/en-us/library/Bb173064(v=VS.85).aspx
	//		.BufferDesc.Height : 화면을 뿌릴 해상도 높이
	//		.BufferDesc.Width : 화면을 뿌릴 해상도 너비
	//		.BufferDesc.Format : 버퍼의 포맷.(color 비트 수 등)
	//			참고: https://docs.microsoft.com/en-us/windows/desktop/api/dxgiformat/ne-dxgiformat-dxgi_format
	//		.BufferDesc.ScanlineOrdering : 주사선 그리기 모드를 설명하는 멤버
	//			참고: https://msdn.microsoft.com/en-us/library/bb173067(v=vs.85).aspx
	//		.BufferDesc.Scaling : 크기 조절모드를 설명하는 멤버
	//			참고 : https://msdn.microsoft.com/en-us/library/bb173066(v=vs.85).aspx
	//
	//	.BufferUsage : 지표 및 자원 작성 옵션용 플레그
	//		참고: https://docs.microsoft.com/en-us/windows/desktop/direct3ddxgi/dxgi-usage
	//	.SwapEffect : IDXGISwapChain1::Present1 호출 후 디스플레이 표면의 픽셀 처리 옵션
	//		참고: https://docs.microsoft.com/en-us/windows/desktop/api/dxgi/ne-dxgi-dxgi_swap_effect
	//	.OutputWindow : 랜더링 결과를 표시할 윈도우 창의 핸들
	//	.Windowed : 윈도우 모드(true) or 전체 화면(false)
	//	
	//	[SampleDesc] : Multi sampling을 위해 추출할 표본 개수와 품질 수준을 서술하는 구조체
	//	멀티샘플링(Multi sampling) :
	//		픽셀 매트릭스로 이미지를 표현할 때 나타나는 거친 이미지를 부드럽게 해주는 기술
	//		대표적으로 안티 알리어싱(Anti - Aliasing)
	//		그래픽 카드에 부하가 어느정도 생김.
	//==============================================================================

	// 스왑체인 구조체를 초기화
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				  //스캔 라인 순서 및 크기를 지정하지 않음으로 설정
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;							  //출력된 다음 백버퍼를 비우게
	swapChainDesc.Flags = 0;

	if (_vsync_enabled) //화면 해상도에 따른 주사율입니다. 지원하지 않으면 문제가 생김
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	if (bFullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// 피처 레벨 지정 : D3D_FEATURE_LEVEL_11_0 (Direct 11) 
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	// D3D11CreateDeviceAndSwapChain
	//	swapChainDesc를 바탕으로 device 와 swapChain을 제작하여 반환
	// TODO: 여유가 생긴다면 파라미터별 내용이 뭔지 정리
	assert(
		D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			&featureLevel,
			1,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&_swapChain,
			&_device,
			NULL,
			&_deviceContext)
		== S_OK);

	ID3D11Texture2D* backBufferPtr; //swapChain으로 부터 백버퍼의 포인터 받아오기
	assert(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr) == S_OK);
	//백버퍼 포인터로 렌더 타겟 뷰를 생성
	assert(_device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView) == S_OK);
	//백 버퍼 해제
	backBufferPtr->Release();
	backBufferPtr = 0;

	//==============


		// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	assert(_device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer) == S_OK);


	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	assert(_device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState) == S_OK);

	// Set the depth stencil state.
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	// Set up the description of the depth disabled stencil state.
	depthStencilDesc.DepthEnable = false;

	// Create the depth stencil state.
	assert(_device->CreateDepthStencilState(&depthStencilDesc, &_depthDisabledStencilState) == S_OK);

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	assert(_device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView) == S_OK);

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	_device->CreateRasterizerState(&rasterDesc, &_rasterState);

	// Now set the rasterizer state.
	_deviceContext->RSSetState(_rasterState);

	// Setup the viewport for rendering.
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	float fieldOfView, screenAspect;
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&_worldMatrix);


	//2D 렌더링에 필요한 orthographic(직교) projection matrix를 제작
	D3DXMatrixOrthoLH(&_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	// Clear the blend state description.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; 
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; 
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; 
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; 
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;



	// Create the blend state using the description.
	assert(_device->CreateBlendState(&blendStateDescription, &_alphaEnabledBlendState) == S_OK);

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	assert(_device->CreateBlendState(&blendStateDescription, &_alphaDisabledBlendState) == S_OK);


	return S_OK;
}
void DxManager::VsyncChecker(int screenWidth, int screenHeight)
{
	_vsync_enabled = false;

	// Direct X 그래픽 인터페이스 팩토리를 생성
	IDXGIFactory* factory;
	assert(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory) == S_OK);

	// 팩토리 객체를 사용하여 첫번째 그래픽 카드 인터페이스 어뎁터를 생성
	IDXGIAdapter* adapter;
	assert(factory->EnumAdapters(0, &adapter) == S_OK);

	//출력(모니터)에 대한 첫번째 어뎁터를 지정
	IDXGIOutput* adapterOutput;
	assert(adapter->EnumOutputs(0, &adapterOutput) == S_OK);

	unsigned int numModes;
	DXGI_MODE_DESC* displayModeList;

	//출력(모니터)에 대한 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수를 가져옵니다.
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	// 가능한 모든 모니터와 그래픽 카드 조합을 저장할 리스트를 생성

	displayModeList = new DXGI_MODE_DESC[numModes];
	//디스 플레이 모드들을 리스트에 채움
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

	// 이제 모든 디스플레이 모드에 대해 화면 너비/ 높이에 맞는 디스플레이 모드를 찾는다.
	//적합한 것을 찾으면 모니터의 새로고침 비율의 분모와 분자 값을 저장
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				_vsync_enabled = true;
			}
		}
	}

	// 디스플레이 모드 리스트 해제
	delete[] displayModeList;
	displayModeList = 0;

	//==================================================================
	//		## 비디오 카드 ## (글카 이야기하는건가?) 
	//==================================================================

	//비디오 카드의 구조체를 얻는다
	DXGI_ADAPTER_DESC adapterDesc;
	assert(adapter->GetDesc(&adapterDesc) == S_OK);

	//비디오 카드 메모리 용량 단위를 메가바이트 단위로 저장
	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 비디오 카드 이름을 저장
	size_t stringLength;
	assert(wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128) == 0);
	// wcstombs_s : 성공시 0, 실패시 오류 코드.
	// https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/wcstombs-s-wcstombs-s-l?view=vs-2019

	//출력 어뎁터, 어뎁터, 팩토리 객체	 해제 
	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	return;
}
void DxManager::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (_swapChain)
	{
		_swapChain->SetFullscreenState(false, NULL);
	}

	if (_alphaEnabledBlendState)
	{
		_alphaEnabledBlendState->Release();
		_alphaEnabledBlendState = 0;
	}

	if (_alphaDisabledBlendState)
	{
		_alphaDisabledBlendState->Release();
		_alphaDisabledBlendState = 0;
	}

	if (_depthDisabledStencilState)
	{
		_depthDisabledStencilState->Release();
		_depthDisabledStencilState = 0;
	}

	if (_rasterState)
	{
		_rasterState->Release();
		_rasterState = 0;
	}

	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if (_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}

	if (_deviceContext)
	{
		_deviceContext->Release();
		_deviceContext = 0;
	}

	if (_device)
	{
		_device->Release();
		_device = 0;
	}

	if (_swapChain)
	{
		_swapChain->Release();
		_swapChain = 0;
	}

	return;
}

void DxManager::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	_deviceContext->ClearRenderTargetView(_renderTargetView, color);

	// Clear the depth buffer.
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void DxManager::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (_vsync_enabled)
	{
		// Lock to screen refresh rate.
		_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		_swapChain->Present(0, 0);
	}

	return;
}
HWND DxManager::GethWnd()
{
	return _hWnd;
}
D3DXVECTOR2 DxManager::GetScreenSize()
{
	return _screenSize;
}
void DxManager::TurnZBufferOn()
{
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
}
void DxManager::TurnZBufferOff()
{
	_deviceContext->OMSetDepthStencilState(_depthDisabledStencilState, 1);
}
void DxManager::EnableAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	_deviceContext->OMSetBlendState(_alphaEnabledBlendState, blendFactor, 0xffffffff);

	return;
}
void DxManager::DisableAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	_deviceContext->OMSetBlendState(_alphaDisabledBlendState, blendFactor, 0xffffffff);
}
//==================================================================
//		## 해상도 크기 ## 
//==================================================================
void DxManager::SetWindowSize(int width, int height)
{
	DXGI_SWAP_CHAIN_DESC* swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	if (_swapChain->GetDesc(swapChainDesc) == S_OK)
	{
		swapChainDesc->BufferDesc.Width = width;
		swapChainDesc->BufferDesc.Height = height;
	}
}
