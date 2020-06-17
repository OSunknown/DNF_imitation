#pragma once
#include "BaseCharacterObject.h"
class CharacterSelectButtonObject :
	public GameObject
{
public:
	CharacterSelectButtonObject();
	~CharacterSelectButtonObject();
public:
	void Init(int slotNumber, float PosX, float PosY);
	void Init(int slotNumber, float PosX, float PosY, float width, float height);

	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void Frame();
	void Shutdown();
public:
	int _slotNumber;
	float _width;
	float _height;

	int mouseX;
	int mouseY;

	bool btnOK = false;
	BaseCharacterObject* slotCharacter;
	ImageObject* slotSelectImage;
};

