#pragma once
class Scene
{
public:
	Scene();
	~Scene();
public:
	virtual void Init();
	virtual void Update();
	virtual void FrameUpdate();
	virtual void Shutdown();
public:
	float Top();
	float Left();
	float Right();
	float Buttom();

	void SetScreenLimit(float top, float left, float right, float buttom);
private:
	float _topLimit;
	float _LeftLimit;
	float _RightLimit;
	float _ButtomLimit;
};

