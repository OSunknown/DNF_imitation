#include "stdafx.h"
#include "Animator.h"


Animator::Animator()
{
	_currentAnimation = NULL;
	_target = NULL;
}


Animator::~Animator()
{
}

void Animator::Frame()
{
	CheckTransition();
	if (_spriteNumber == (int)_currentAnimation->frame.size() - 1 && _currentAnimation->isLoop == false)
		return;

	if (_spriteNumber == _currentAnimation->frame.size() - 1 && _currentAnimation->isLoop == true)
	{
		_spriteNumber = 0;
	}
	if (_currentAnimation->frame[_spriteNumber] < _frameCount)
	{
		_frameCount = _frameCount - _currentAnimation->frame[_spriteNumber];
		_spriteNumber++;
	}
	_target->SetSprite(_currentAnimation->texture[_spriteNumber]);
	_frameCount += Time.GetDeltaTime(); //델타 타임이 비정상적이게 작동하므로 고쳐야됨
}

void Animator::SetBool(string key, bool value)
{
	_umbPrameta[_mPrametahash[key]] = value;
}

void Animator::SetBool(int key, bool value)
{
	_umbPrameta[key] = value;
}

void Animator::SetInteger(string key, int value)
{
	_umiPrameta[_mPrametahash[key]] = value;
}

void Animator::SetInteger(int key, int value)
{
	_umiPrameta[key] = value;
}

void Animator::SetFloat(string key, float value)
{
	_umfPrameta[_mPrametahash[key]] = value;
}

void Animator::SetFloat(int key, float value)
{
	_umfPrameta[key] = value;
}

bool Animator::GetBool(string key)
{
	return _umbPrameta[_mPrametahash[key]];
}

bool Animator::GetBool(int key)
{
	return _umbPrameta[key];
}

int Animator::GetInteger(string key)
{
	return _umiPrameta[_mPrametahash[key]];
}

int Animator::GetInteger(int key)
{
	return _umiPrameta[key];
}

float Animator::GetFloat(string key)
{
	return _umfPrameta[_mPrametahash[key]];
}

float Animator::GetFloat(int key)
{
	return _umfPrameta[key];
}

void Animator::SetTarget(SpriteObject * target)
{
	_target = target;
}

void Animator::AddNewParameta(string name, bool parameta)
{
	int count = _umbPrameta.size();
	_mPrametahash.insert(make_pair(name, count));
	_umbPrameta.insert(make_pair(count,parameta));
}

void Animator::AddNewParameta(string name, int parameta)
{
	int count = _umiPrameta.size();
	_mPrametahash.insert(make_pair(name, count));
	_umiPrameta.insert(make_pair(count, parameta));
}

void Animator::AddNewParameta(string name, float parameta)
{
	int count = _umfPrameta.size();
	_mPrametahash.insert(make_pair(name, count));
	_umfPrameta.insert(make_pair(count, parameta));
}

void Animator::AddClip(string name, Animation * animetion)
{
	_mAnimations.insert(make_pair(name, animetion));
}

const size_t Animator::StringToHash(string name)
{
	return _mPrametahash[name];
}

void Animator::MakeTransition(string animationName, Transition trans)
{
	if (_Transitions.find(animationName) == _Transitions.end())
	{
		vector<Transition> vTrans;
		vTrans.push_back(trans);
		_Transitions.insert(make_pair(animationName, vTrans));
	}
	else
	{
		_Transitions[animationName].push_back(trans);
	}
	
}

void Animator::CheckTransition()
{
	vector<Transition> vTrans = _Transitions[_currentName];
	for (size_t i = 0; i < vTrans.size(); i++)
	{
		Transition trans = vTrans[i];
		if (trans.HasExitTime == true && (_spriteNumber != _currentAnimation->frame.size() - 1))
		{
			continue;
		}
		bool bTransTrue = true;
		for (size_t j = 0; j < trans.conditions.size(); j++)
		{
			switch (trans.conditions[j].type)
			{
			case 0://bool
				if (_umbPrameta[trans.conditions[j].namehash] != (bool)trans.conditions[j].parameta)
				{
					bTransTrue = false;
				}
				break;
			case 1: //int 
				if (trans.conditions[j].conditional == 0 && (_umiPrameta[trans.conditions[j].namehash] >= (int)trans.conditions[j].parameta))
				{
					bTransTrue = false;
				}
				//같을때 true
				if (trans.conditions[j].conditional == 1 && (_umiPrameta[trans.conditions[j].namehash] != (int)trans.conditions[j].parameta))
				{
					bTransTrue = false;
				}
				//보다 클때
				if (trans.conditions[j].conditional == 2 && (_umiPrameta[trans.conditions[j].namehash] <= (int)trans.conditions[j].parameta))
				{
					bTransTrue = false;
				}
				break;
			case 2: //float
				//cout << _currentName +" "+ trans.targetAnime + " " + trans.conditions[j].name + " " << _fPrameta[trans.conditions[j].name] << " " << trans.conditions[j].parameta << endl;
				//작을때 true
				if (trans.conditions[j].conditional == 0 && (_umfPrameta[trans.conditions[j].namehash] >= trans.conditions[j].parameta))
				{
					bTransTrue = false;
				}
				//같을때 true
				if (trans.conditions[j].conditional == 1 && (_umfPrameta[trans.conditions[j].namehash] != trans.conditions[j].parameta))
				{
					bTransTrue = false;
				}
				//보다 클때
				if (trans.conditions[j].conditional == 2 && (_umfPrameta[trans.conditions[j].namehash] <= trans.conditions[j].parameta))
				{
					bTransTrue = false;
				}
				break;
			}
		}
		if (bTransTrue == true)
		{
			_currentName = trans.targetAnime;
			_currentAnimation = _mAnimations[trans.targetAnime];
			_spriteNumber = 0;
			_frameCount = 0;
		}
		
	}
}

void Animator::createAnimetion(bool isLoop,vector<float> frame, vector<string> spritenames)
{
	_currentAnimation = new Animation();
	_currentAnimation->isLoop = isLoop;
	_currentAnimation->frame = frame;
	for (size_t i = 0; i < spritenames.size(); i++)
	{
		_currentAnimation->texture.push_back(IMAGEMANAGER.GetSceneSprite(spritenames[i]));
	}
}

float Animator::GetFrame()
{
	return _frameCount;
}

int Animator::GetSpriteNumber()
{
	return _spriteNumber;
}

