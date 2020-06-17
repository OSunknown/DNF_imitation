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
	// ��������ȭ 
	if (bVsync)
	{
		VsyncChecker(screenWidth, screenHeight);
		if (_vsync_enabled == false)
		{
			if (MessageBox(hWnd,
				"����Ͱ� �ش� �ػ󵵸� �������� �ʾƿ� vsync���� ��� �����Ͻ�?",
				"Vsync Failure", MB_YESNO) == IDNO)
			{
				// TODO:���� ���� ������ �ּ���.
			}
		}
	}

	//========================[DXGI_SWAP_CHAIN_DESC]================================
	//	.BufferCount : �� ������ ����.(���߹��۸� : 1��, ���߹��۸� : 2��)
	//		1. [1�� ����] -> ȭ�鿡 �Ѹ�. [2�� ����] -> �׸�
	//		2. [1�� ����] <- ��ü -> [2�� ����]
	//		3. [2�� ����] -> ȭ�鿡 �Ѹ�. [1�� ����] -> �׸�
	//
	//	[BufferDesc] : �����ϰ��� �ϴ� Back buffer�� �Ӽ����� �����ϴ� ����ü
	//		����: https://msdn.microsoft.com/en-us/library/Bb173064(v=VS.85).aspx
	//		.BufferDesc.Height : ȭ���� �Ѹ� �ػ� ����
	//		.BufferDesc.Width : ȭ���� �Ѹ� �ػ� �ʺ�
	//		.BufferDesc.Format : ������ ����.(color ��Ʈ �� ��)
	//			����: https://docs.microsoft.com/en-us/windows/desktop/api/dxgiformat/ne-dxgiformat-dxgi_format
	//		.BufferDesc.ScanlineOrdering : �ֻ缱 �׸��� ��带 �����ϴ� ���
	//			����: https://msdn.microsoft.com/en-us/library/bb173067(v=vs.85).aspx
	//		.BufferDesc.Scaling : ũ�� ������带 �����ϴ� ���
	//			���� : https://msdn.microsoft.com/en-us/library/bb173066(v=vs.85).aspx
	//
	//	.BufferUsage : ��ǥ �� �ڿ� �ۼ� �ɼǿ� �÷���
	//		����: https://docs.microsoft.com/en-us/windows/desktop/direct3ddxgi/dxgi-usage
	//	.SwapEffect : IDXGISwapChain1::Present1 ȣ�� �� ���÷��� ǥ���� �ȼ� ó�� �ɼ�
	//		����: https://docs.microsoft.com/en-us/windows/desktop/api/dxgi/ne-dxgi-dxgi_swap_effect
	//	.OutputWindow : ������ ����� ǥ���� ������ â�� �ڵ�
	//	.Windowed : ������ ���(true) or ��ü ȭ��(false)
	//	
	//	[SampleDesc] : Multi sampling�� ���� ������ ǥ�� ������ ǰ�� ������ �����ϴ� ����ü
	//	��Ƽ���ø�(Multi sampling) :
	//		�ȼ� ��Ʈ������ �̹����� ǥ���� �� ��Ÿ���� ��ģ �̹����� �ε巴�� ���ִ� ���
	//		��ǥ������ ��Ƽ �˸����(Anti - Aliasing)
	//		�׷��� ī�忡 ���ϰ� ������� ����.
	//==============================================================================

	// ����ü�� ����ü�� �ʱ�ȭ
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
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				  //��ĵ ���� ���� �� ũ�⸦ �������� �������� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;							  //��µ� ���� ����۸� ����
	swapChainDesc.Flags = 0;

	if (_vsync_enabled) //ȭ�� �ػ󵵿� ���� �ֻ����Դϴ�. �������� ������ ������ ����
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

	// ��ó ���� ���� : D3D_FEATURE_LEVEL_11_0 (Direct 11) 
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	// D3D11CreateDeviceAndSwapChain
	//	swapChainDesc�� �������� device �� swapChain�� �����Ͽ� ��ȯ
	// TODO: ������ ����ٸ� �Ķ���ͺ� ������ ���� ����
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

	ID3D11Texture2D* backBufferPtr; //swapChain���� ���� ������� ������ �޾ƿ���
	assert(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr) == S_OK);
	//����� �����ͷ� ���� Ÿ�� �並 ����
	assert(_device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView) == S_OK);
	//�� ���� ����
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


	//2D �������� �ʿ��� orthographic(����) projection matrix�� ����
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

	// Direct X �׷��� �������̽� ���丮�� ����
	IDXGIFactory* factory;
	assert(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory) == S_OK);

	// ���丮 ��ü�� ����Ͽ� ù��° �׷��� ī�� �������̽� ��͸� ����
	IDXGIAdapter* adapter;
	assert(factory->EnumAdapters(0, &adapter) == S_OK);

	//���(�����)�� ���� ù��° ��͸� ����
	IDXGIOutput* adapterOutput;
	assert(adapter->EnumOutputs(0, &adapterOutput) == S_OK);

	unsigned int numModes;
	DXGI_MODE_DESC* displayModeList;

	//���(�����)�� ���� DXGI_FORMAT_R8G8B8A8_UNORM ǥ�� ���Ŀ� �´� ��� ���� �����ɴϴ�.
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	// ������ ��� ����Ϳ� �׷��� ī�� ������ ������ ����Ʈ�� ����

	displayModeList = new DXGI_MODE_DESC[numModes];
	//�� �÷��� ������ ����Ʈ�� ä��
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

	// ���� ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/ ���̿� �´� ���÷��� ��带 ã�´�.
	//������ ���� ã���� ������� ���ΰ�ħ ������ �и�� ���� ���� ����
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

	// ���÷��� ��� ����Ʈ ����
	delete[] displayModeList;
	displayModeList = 0;

	//==================================================================
	//		## ���� ī�� ## (��ī �̾߱��ϴ°ǰ�?) 
	//==================================================================

	//���� ī���� ����ü�� ��´�
	DXGI_ADAPTER_DESC adapterDesc;
	assert(adapter->GetDesc(&adapterDesc) == S_OK);

	//���� ī�� �޸� �뷮 ������ �ް�����Ʈ ������ ����
	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// ���� ī�� �̸��� ����
	size_t stringLength;
	assert(wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128) == 0);
	// wcstombs_s : ������ 0, ���н� ���� �ڵ�.
	// https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/wcstombs-s-wcstombs-s-l?view=vs-2019

	//��� ���, ���, ���丮 ��ü	 ���� 
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
//		## �ػ� ũ�� ## 
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
