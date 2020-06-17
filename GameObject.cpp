#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
{
	_depth = 0;
	isTransfromChange = false;
	_originalsize = 0.0f;
}

GameObject::~GameObject()
{
}

void GameObject::Awake()
{	
}

void GameObject::Start()
{
}

void GameObject::Update()
{
}

void GameObject::FixedUpdate()
{
}
void GameObject::Frame()
{
}

void GameObject::Shutdown()
{
}

void GameObject::SetTransform()
{
	isTransfromChange = true;
	_position = D3DXVECTOR2(0, 0);
	_depth = 0;
	_scale = D3DXVECTOR3(1, 1, 1);
	_rotation = D3DXVECTOR3(0, 0, 0);
}

void GameObject::SetTransform(D3DXVECTOR2 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	isTransfromChange = true;
	_position = position;
	_rotation = rotation;
	_scale = scale;
	_originalsize = 1.0f / scale.x;
}

void GameObject::SetPosition(D3DXVECTOR2 position)
{
	isTransfromChange = true;
	_position = position;
}

void GameObject::SetRotation(D3DXVECTOR3 rotation)
{
	isTransfromChange = true;
	_rotation = rotation;
}

void GameObject::SetScale(D3DXVECTOR3 scale)
{
	isTransfromChange = true;
	_scale = scale;
}

void GameObject::SetDepth(int depth)
{
	_depth = depth;
}

D3DXMATRIX GameObject::GetWorldMatrix()
{
	if (isTransfromChange == false)
		return _worldMatrix;

	D3DXMatrixIdentity(&_worldMatrix);

	D3DXMATRIX SMatrix, RMatrix, TMatrix;

	D3DXMatrixScaling(&SMatrix, _scale.x, _scale.y, _scale.z);
	D3DXVECTOR3 rotation = _rotation * 3.141592f / 180.0f;
	D3DXMatrixRotationYawPitchRoll(&RMatrix, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&TMatrix, _position.x, _position.y, 0);

	_worldMatrix = SMatrix * RMatrix * TMatrix;
	isTransfromChange = false;

	return _worldMatrix;
}

