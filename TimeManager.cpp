#include "stdafx.h"
#include "TimeManager.h"
#pragma comment(lib, "winmm.lib")

#include <mmsystem.h>


TimeManager::TimeManager()
{
	_firstime = 0;
	_lasttime = 0;
	_FPSChecker = 0;
	_deltatimeChecker = 0;
	_deltatime = 0;
	_deltatimelast = 0;
	_fpsCount = 0;
	_fpsLimit = 0;
}


TimeManager::~TimeManager()
{
}

void TimeManager::Init(int fpsLimit)
{
	_FPSChecker = timeGetTime();
	_firstime = timeGetTime();
	_fpsLimit = (1.0f / (float)fpsLimit);
	_deltatime = _fpsLimit;
	_fpsCount = 0;
}

void TimeManager::Update()
{
	_lasttime = timeGetTime();
	_deltatimeChecker = (_lasttime - _firstime) * 0.001f;
	
	/*_deltatime = (timeGetTime() - _deltatimelast) * 0.001f;
	_deltatimelast = timeGetTime();*/

	if (_lasttime > (_FPSChecker + 1000))
	{
		_fpsCount = 0;
		_FPSChecker = _lasttime;
	}
}

bool TimeManager::CheckFPSLimit()
{
	if (_deltatimeChecker >= _fpsLimit)
	{
		_fpsCount++;
		_firstime = _lasttime;
		_deltatime = _deltatimeChecker;
		return true;
	}else
		return false;
}
