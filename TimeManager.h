#pragma once
class TimeManager
{
public:
	TimeManager();
	~TimeManager();
	static TimeManager & inse() { static TimeManager insetance; return insetance; };
	void Init(int fpsLimit);
	void Update();
	
	float GetDeltaTime() { return _deltatime; };
	bool CheckFPSLimit();
private:
	unsigned long _firstime;
	unsigned long _lasttime;
	unsigned long _FPSChecker;
	float _deltatimeChecker;
	float _deltatime;
	unsigned long _deltatimelast;
	int _fpsCount;
	float _fpsLimit;

};

