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
	//bool HasExitTime	 : 애니메이션이 진행중이라면 도중에 다른 조건이 와도 현재 애니매이션이 끝날때까지 이쪽 길은 사용하지 않음.
	bool HasExitTime = false;

	vector<Conditions> conditions;
	/*
		AddCondition
			string name			 : 파라미터에서 호출할 이름
			int type			 : 파라미터 타입
				0 = bool
				1 = int
				2 = float
			int conditional 	 : 파라미터 조건
				0 = 들어온 parameta 기 체크할 값 보다 작을때
				1 = 들어온 parameta 와 체크할 값이 같을 때
				2 = 들어온 parameta 가 체크할 값 보다 클때
				type 이 bool(0)일떄는 무시
			float parameta		 : 파라미터
				type 이 bool(0)이라면
					0이 false
					1이 true;
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
	
	//Prameta 관련
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
	
	//scene에 배치될 움직이는 사물용입니다.
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