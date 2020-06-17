#include "stdafx.h"
#include "RenderManager.h"



RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}

void RenderManager::Init(void)
{
	_TextureShader = new TextureShaderClass;
	_TextureShader->Initialize(DxM.GetDevice(), DxM.GethWnd());
	
	_SpriteShader = new SpriteShaderClass;
	_SpriteShader->Initialize(DxM.GetDevice(), DxM.GethWnd());
}

void RenderManager::TextureShader(int indexCount, D3DXMATRIX worldMatrix, ID3D11ShaderResourceView * texture)
{
	D3DXMATRIX viewMatrix, orthoMatrix;
	MainCamera.GetViewMatrix(viewMatrix);
	DxM.GetOrthoMatrix(orthoMatrix);
	_TextureShader->Render(DxM.GetDeviceContext(), indexCount, worldMatrix, viewMatrix, orthoMatrix, texture);
}

void RenderManager::SpriteShader(int indexCount, int instanceCount, D3DXMATRIX worldMatrix, ID3D11ShaderResourceView * texture)
{
	D3DXMATRIX viewMatrix, orthoMatrix;
	MainCamera.GetViewMatrix(viewMatrix);
	DxM.GetOrthoMatrix(orthoMatrix);
	_SpriteShader->Render(DxM.GetDeviceContext(), indexCount, instanceCount, worldMatrix, viewMatrix, orthoMatrix, texture);
}
