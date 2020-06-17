#pragma once
#include "GameObject.h"
enum class slotType
{
	WEAPON,
	HEADGEAR,
	HAIR,
	FACE,
	SKIN,
	JACKET,
	PANTS,
	SHOES,
	BREAST,
	WAIST,
};
class EquipmentObject :
	public GameObject
{
public:
	EquipmentObject();
	~EquipmentObject();
public: //frame»©°í ¾È¾¸
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void Frame();
	void Shutdown();
public:
	void SetUiType();
	void SetWorldObjectType();
public:
	void Init();
	void Play();
public:
	//Animation µé
	void Idle();
	void Walk();
	void Run();

	void Jump();

	void defultAttack();
	void defultAttackRelease();
	void Skill1();
	void Skill2();

	void SkillSlot1();
	void SkillSlot2();
	void SkillSlot3();
	void SkillSlot4();
	void SkillSlot5();
	void SkillSlot6();

	void ExpansionSkillSlot1();
	void ExpansionSkillSlot2();
	void ExpansionSkillSlot3();
	void ExpansionSkillSlot4();
	void ExpansionSkillSlot5();
	void ExpansionSkillSlot6();

	void SetAnimation(string name, int depth);
	void SetSlot(slotType slottype);
private:
	SpriteObject* _currentsprite;
	Animator _currentAnimator;
	D3DXVECTOR2 _moveInput;
	slotType _slottype;
	
};

