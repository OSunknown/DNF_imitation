#pragma once

class  EquipmentObject;
class BaseCharacterObject :
	public GameObject
{
public:
	BaseCharacterObject();
	~BaseCharacterObject();
public: // 상속된
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Frame();
	virtual void Shutdown();
public:
	void init(int slotNum);
	void ChangeEquipment(string name);
	//
	void SetUiType();
	void SetWorldObjectType();
	void SetFlip(bool flip);
	//Animation 들
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

	void itemSlot1();
	void itemSlot2();
	void itemSlot3();
	void itemSlot4();
	void itemSlot5();
	void itemSlot6();
public:
	//아바타를 장착한다면 여기서 바꿀것.
	//EquipmentObject는Animator와 SpriteObject 를 가진 오브젝트임)
	vector<EquipmentObject> equipEquipment;
	bool EquipmentChange;
	int _slotNum;
	string jobID;
};
