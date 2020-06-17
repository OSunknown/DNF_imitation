#pragma once
#include "Scene.h"
#include "Animator.h"
class ElvenguardScene :
	public Scene
{
public:
	ElvenguardScene();
	~ElvenguardScene();
public:
	void Init();
	void Update();
	void FrameUpdate();
	void Shutdown();
	void CreateSpriteObject(string name, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	void CreateSpriteObjectAni(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale, vector<float> frame, vector<string> names);
private:
	vector<GameObject *> Background;
	vector<GameObject *> GameObjects;
	vector<GameObject *> Nearground;
	vector<Animator> animators;
	vector<SpriteObject*> backgrounds; //¹è°æ
	int _nearNumber;
};

