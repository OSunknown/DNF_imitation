#pragma once

struct Conditions
{
	int namehash;
	int type;
	int conditional;
	float parameta;
	Conditions(int _namehash, int _type, int _conditional, float _parameta)
	{
		namehash = _namehash;
		type = _type;
		conditional = _conditional;
		parameta = _parameta;
	}
};
struct Transition
{
	string targetAnime;
	//bool HasExitTime	 : �ִϸ��̼��� �������̶�� ���߿� �ٸ� ������ �͵� ���� �ִϸ��̼��� ���������� ���� ���� ������� ����.
	bool HasExitTime = false;

	vector<Conditions> conditions;
	/*
		AddCondition
			string name			 : �Ķ���Ϳ��� ȣ���� �̸�
			int type			 : �Ķ���� Ÿ��
				0 = bool
				1 = int
				2 = float
			int conditional 	 : �Ķ���� ����
				0 = ���� parameta �� üũ�� �� ���� ������
				1 = ���� parameta �� üũ�� ���� ���� ��
				2 = ���� parameta �� üũ�� �� ���� Ŭ��
				type �� bool(0)�ϋ��� ����
			float parameta		 : �Ķ����
				type �� bool(0)�̶��
					0�� false
					1�� true;
	*/
	void AddCondition(int namehash, int type, int conditional, float parameta)
	{
		conditions.push_back(Conditions(namehash, type, conditional, parameta));
	}
};
class Animator
{
public:
	Animator();
	~Animator();
public:
	
	//Prameta ����
	void SetBool(string key, bool value);
	void SetBool(int key, bool value);
	void SetInteger(string key, int value);
	void SetInteger(int key, int value);
	void SetFloat(string key, float value);
	void SetFloat(int key, float value);

	bool GetBool(string key);
	bool GetBool(int key);
	int GetInteger(string key);
	int GetInteger(int key);
	float GetFloat(string key);
	float GetFloat(int key);
	//
	void AddNewParameta(string name, bool parameta);
	void AddNewParameta(string name, int parameta);
	void AddNewParameta(string name, float parameta);
	//
	void AddClip(string name, Animation* animetion);
	//
	const size_t StringToHash(string name);
	//
	void Frame();

	void SetTarget(SpriteObject * target);
	
	void MakeTransition(string animationName, Transition trans);

	void CheckTransition();

	void Play(string name)
	{
		_currentName = name;
		_currentAnimation = _mAnimations[name];
		int i = 0;
	}
	
	//scene�� ��ġ�� �����̴� �繰���Դϴ�.
	void createAnimetion(bool isLoop, vector<float> frame, vector<string> spritenames);

	float GetFrame();
	int GetSpriteNumber();
	
private:
	unordered_map<int, bool, bypass> _umbPrameta;
	unordered_map<int,int,bypass> _umiPrameta;
	unordered_map<int, float, bypass> _umfPrameta;

	map<string, size_t> _mPrametahash;

	map<string, Animation*> _mAnimations;


	string _currentName;
	Animation* _currentAnimation;
	
	float _frameCount = 0;
	int _spriteNumber = 0;

	SpriteObject * _target;
	unordered_map<string, vector<Transition>> _Transitions;
};