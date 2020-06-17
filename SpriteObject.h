#pragma once
class SpriteObject
{
private:
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
	SpriteObject();
	~SpriteObject();
	void Init();
	void Init(string name, D3DXVECTOR3 position , D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	void Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	void Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale, Animator controller);
	bool Render();
	void Shutdown();
//set	
	void SetPosition(D3DXVECTOR3 position);
	void SetRotation(D3DXVECTOR3 rotation);
	void SetScale(D3DXVECTOR3 scale);

	void SetSprite(sprite* texture);
//add
	void AddOriginalPosition(D3DXVECTOR3 position);
//get
	int GetIndexCount();
	D3DXVECTOR3 GetPosition();
	D3DXMATRIX GetWorldMatrix();
	ID3D11ShaderResourceView * GetTexture();
public:
	bool Filp = false;
	bool UIType = false;
	
private:
	bool InitializeBuffers();
	void RenderBuffers();
	void SetRect();
private:
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _rotation;
	D3DXVECTOR3 _scale;
	D3DXMATRIX _worldMatrix;

	bool _isTransfromChange;
	
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	int m_vertexCount, m_indexCount;	
	float left, right, top, bottom;

	sprite* _currentsprite;
	
};
