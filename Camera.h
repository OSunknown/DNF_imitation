#pragma once


class Camera
{
public:
	Camera();
	~Camera();
	static Camera & inse() { static Camera instance; return instance; }
public:
	void Init(void);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	void SetPostion(float x, float y, float z);
	void SetPostion(D3DXVECTOR3 position);
	void trackPosition(D3DXVECTOR2 position);
	void SetRotation(float x, float y, float z);
	void SetLimit(float top, float left, float right, float buttom);
	void AddPosition(float x, float y, float z);
	void Render();
	void GetViewMatrix(D3DXMATRIX& viewmatrix);
	D3DXVECTOR2 ScreenToWorldPoint(D3DXVECTOR2 screenPoint);
	D3DXVECTOR2 WorldToScreenPoint(D3DXVECTOR2 worldPoint);
private:
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _rotation;
	D3DXMATRIX _viewMatrix;
	float _top;
	float _left;
	float _right;
	float _bottom;
};

