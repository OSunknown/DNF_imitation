#pragma once
#include "GameObject.h"
class BackgroundAniObject :
	public GameObject
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	struct InstanceType
	{
		D3DXVECTOR3 position;
	};
public:
	BackgroundAniObject();
	~BackgroundAniObject();
public:
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void Frame();
	void Shutdown();
public:
	bool InitializeBuffers();

	int GetVertexCount();
	int GetIndexCount();

	void SetAnimation(string name, int depth);

	ID3D11ShaderResourceView * GetTexture();
public:
	sprite * _currentsprite;

	int _vertexCount;
	int _indexCount;
	ID3D11Buffer *_vertexBuffer;
	ID3D11Buffer *_indexBuffer;
	float left, right, top, bottom;

	Animation* _currentAnimation;
	float _frameCount = 0;
	int _spriteNumber = 0;

};

