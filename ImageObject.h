#pragma once
//==================================================================
//		## ImageObject ## (UI용 SpriteObject)
//==================================================================
/*
	화면의 위치를 잡아서 하기때문에 Left는 화면의 중간/2를 뺀값으로?

*/
class ImageObject
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
	ImageObject();
	~ImageObject();
	void Init();
	void Init(string name, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	void Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	void Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale, Animator controller);
	bool Render();
	void Shutdown();
	//set	
	void SetPosition(D3DXVECTOR3 position);
	void SetRotation(D3DXVECTOR3 rotation);
	void SetScale(D3DXVECTOR3 scale);
	//add
	void AddOriginalPosition(D3DXVECTOR3 position);
	//get
	int GetIndexCount();
	D3DXVECTOR3 GetPosition();
	D3DXMATRIX GetWorldMatrix();
	ID3D11ShaderResourceView* GetTexture();
public:
	void SetRect();
	float left, right, top, bottom;
public:
	bool Filp = false;
	sprite* _currentsprite;

private:
	bool InitializeBuffers();
	void RenderBuffers();
	
private:
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _rotation;
	D3DXVECTOR3 _scale;
	D3DXMATRIX _worldMatrix;

	bool isTransfromChange;

	ID3D11Buffer* _vertexBuffer, * _indexBuffer;
	int m_vertexCount, m_indexCount;
	
};

