#pragma once
////==================================================================
/*		
			## BackgroundSpriteObject ## 
 1���� �̹����� ���� ��ġ�� ������ �ѷ��ִ� ��������Ʈ ������Ʈ
 ���� �̹����� �ݺ��Ǵ� ��濡 �����ҵ�
*/
////==================================================================
class BackgroundSpriteObject :
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
	BackgroundSpriteObject();
	~BackgroundSpriteObject();
private:
	sprite * _currentsprite;
	int _vertexCount;
	int _indexCount;
	int _instanceCount;
	ID3D11Buffer *_vertexBuffer;
	ID3D11Buffer *_indexBuffer;
	ID3D11Buffer *_instanceBuffer;
	bool Filp = false;
	float left, right, top, bottom;
	
public:
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void Frame();
	void Shutdown();

public:
	void SetTexture(string name,int depth);

	bool InitializeInstance(vector<D3DXVECTOR3> positions);
	bool InitializeBuffers();

	int GetVertexCount();
	int GetInstanceCount();
	int GetIndexCount();
	ID3D11ShaderResourceView * GetTexture();
private:
	
};

