#pragma once
class EffectObject
{
public:
	EffectObject();
	~EffectObject();
public:
	//
	//isfrontEffect 가 true 라면 케릭터보다 앞(화면에) 있음.
	//startTime 애니메이션의 시작시간
	//endTime 애니메이션의 끝나는 시간 
	//anime 실행될 애니메이션
	void AddAnimation(bool isfrontEffect,float startTime, float endTime, Animation anime);
	//
	bool Rander();
private:
	float EffectTime;

};

