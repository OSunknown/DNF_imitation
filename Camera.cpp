#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	_left = 0;
	_right = 0;
	_top = 0;
	_bottom = 0;
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	_position = D3DXVECTOR3(0, 0, 0);
	_rotation = D3DXVECTOR3(0, 0, 0);
}

D3DXVECTOR3 Camera::GetPosition()
{
	return _position;
}

D3DXVECTOR3 Camera::GetRotation()
{
	return _rotation;
}

void Camera::SetPostion(float x, float y, float z)
{
	_position = D3DXVECTOR3(x, y, z);
}

void Camera::SetPostion(D3DXVECTOR3 position)
{
	_position = position;

	if (position.x > _right)
	{
		_position.x = _right;
	}
	if (position.x < _left)
	{
		_position.x = _left;
	}

	if (position.y > _top)
	{
		_position.y = _top;
	}

	if (position.y < _bottom)
	{
		_position.y = _bottom;
	}

	_position.z = -100;
}

void Camera::SetRotation(float x, float y, float z)
{
	_rotation = D3DXVECTOR3(x, y, z);
}

void Camera::SetLimit(float top, float left, float right, float buttom)
{
	float offsetX = DxM.GetScreenSize().x / 2.0f;
	float offsetY = DxM.GetScreenSize().y / 2.0f;
	_top = top - offsetY;
	_left = left + offsetX;
	_right = right - offsetX;
	_bottom = buttom + offsetY;
}

void Camera::AddPosition(float x, float y, float z)
{
	_position += D3DXVECTOR3(x, y, z);

	if (_position.x > _right)
	{
		_position.x = _right;
	}
	if (_position.x < _left)
	{
		_position.x = _left;
	}

	if (_position.y > _top)
	{
		_position.y = _top;
	}

	if (_position.y < _bottom)
	{
		_position.y = _bottom;
	}
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = D3DXVECTOR3(0, 1, 0);

	// Setup where the camera is looking by default.
	lookAt = D3DXVECTOR3(0, 0, 1);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	//0.0174532925f 는 약 1라디안.
	pitch = _rotation.x * 0.0174532925f;
	yaw = _rotation.y * 0.0174532925f;
	roll = _rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = _position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&_viewMatrix, &_position, &lookAt, &up);
}

void Camera::GetViewMatrix(D3DXMATRIX & viewMatrix)
{ 
	viewMatrix = _viewMatrix;
}

D3DXVECTOR2 Camera::ScreenToWorldPoint(D3DXVECTOR2 screenPoint)
{
	float screenWidth = App.DxscreenWidth / 2.0f;
	float screenHeight = App.DxscreenHeight / 2.0f;
	return D3DXVECTOR2(_position.x + screenPoint.x - screenWidth,_position.y + screenPoint.y - screenHeight);
}
