#pragma once
#include "TextureShaderClass.h"
#include "SpriteShaderClass.h"
class RenderManager
{
public:
	RenderManager();
	~RenderManager();
	static RenderManager & inse() { static RenderManager instance; return instance; };
public:
	void Init(void);
	void TextureShader(int indexCount, D3DXMATRIX worldMatrix, ID3D11ShaderResourceView* texture);
	void SpriteShader(int indexCount,int instanceCount, D3DXMATRIX worldMatrix, ID3D11ShaderResourceView* texture);
private:
	TextureShaderClass * _TextureShader;
	SpriteShaderClass * _SpriteShader;
};

