#pragma once
class GameObject
{
public:
	GameObject();
	~GameObject();
public:
	D3DXVECTOR2 _position;
	D3DXVECTOR3 _rotation;
	D3DXVECTOR3 _scale;
	D3DXMATRIX _worldMatrix;
	int _depth;
	bool isTransfromChange;
	float _originalsize;
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Frame();
	virtual void Shutdown();
public:
	void SetTransform();
	void SetTransform(D3DXVECTOR2 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	//Object position
	void SetPosition(D3DXVECTOR2 position);
	//Object Rotation
	void SetRotation(D3DXVECTOR3 rotation);
	//Object Scale
	void SetScale(D3DXVECTOR3 scale);
	//rendering 순서 높은 값-> 낮은값
	void SetDepth(int depth);

	

	D3DXMATRIX GetWorldMatrix();
};

