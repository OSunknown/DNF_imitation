#pragma once
class EffectObject
{
public:
	EffectObject();
	~EffectObject();
public:
	//
	//isfrontEffect �� true ��� �ɸ��ͺ��� ��(ȭ�鿡) ����.
	//startTime �ִϸ��̼��� ���۽ð�
	//endTime �ִϸ��̼��� ������ �ð� 
	//anime ����� �ִϸ��̼�
	void AddAnimation(bool isfrontEffect,float startTime, float endTime, Animation anime);
	//
	bool Rander();
private:
	float EffectTime;

};

