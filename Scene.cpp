#include "stdafx.h"
#include "Scene.h"



Scene::Scene()
{
	_topLimit = 0;
	_LeftLimit = 0;
	_RightLimit = 0;
	_ButtomLimit = 0;
}


Scene::~Scene()
{
}

void Scene::Init()
{
	return;
}

void Scene::Update()
{
}

void Scene::FrameUpdate()
{
}

void Scene::Shutdown()
{
}

float Scene::Top()
{
	return _topLimit;
}

float Scene::Left()
{
	return _LeftLimit;
}

float Scene::Right()
{
	return _RightLimit;
}

float Scene::Buttom()
{
	return _ButtomLimit;
}

void Scene::SetScreenLimit(float top, float left, float right, float buttom)
{
	_topLimit = top;
	_LeftLimit = left;
	_RightLimit = right;
	_ButtomLimit = buttom;
}
