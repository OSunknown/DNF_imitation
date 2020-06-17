#include "stdafx.h"
#include "EquipmentObject.h"



EquipmentObject::EquipmentObject()
{
	_currentsprite = NULL;
}

EquipmentObject::~EquipmentObject()
{
}

void EquipmentObject::Awake()
{
}

void EquipmentObject::Start()
{
}

void EquipmentObject::Update()
{
}

void EquipmentObject::FixedUpdate()
{
}

void EquipmentObject::Frame()
{
	_currentAnimator.Frame();
	_currentsprite->SetPosition(D3DXVECTOR3(_position.x, _position.y, 0));
	DxM.TurnZBufferOff();
	_currentsprite->Render();
	DxM.TurnZBufferOn();
}

void EquipmentObject::Shutdown()
{
}

void EquipmentObject::SetUiType()
{
	_currentsprite->UIType = true;
}

void EquipmentObject::SetWorldObjectType()
{
	_currentsprite->UIType = false;
}

void EquipmentObject::Init()
{
	_currentsprite = new SpriteObject();
	_currentsprite->Init();
}

void EquipmentObject::Play()
{
	_currentAnimator.Play("idle");
}

void EquipmentObject::Idle()
{
	_currentAnimator.SetBool("bIdle", true);
	_currentAnimator.SetFloat("fSpeed", 0);
}

void EquipmentObject::Walk()
{
	_currentAnimator.SetBool("bIdle", false);
	_currentAnimator.SetFloat("fSpeed", 5);
}

void EquipmentObject::Run()
{
	_currentAnimator.SetBool("bIdle", false);
	_currentAnimator.SetFloat("fSpeed", 10);
}

void EquipmentObject::Jump()
{
}

void EquipmentObject::defultAttack()
{
}

void EquipmentObject::defultAttackRelease()
{
}

void EquipmentObject::Skill1()
{
}

void EquipmentObject::Skill2()
{
}

void EquipmentObject::SkillSlot1()
{
}

void EquipmentObject::SkillSlot2()
{
}

void EquipmentObject::SkillSlot3()
{
}

void EquipmentObject::SkillSlot4()
{
}

void EquipmentObject::SkillSlot5()
{
}

void EquipmentObject::SkillSlot6()
{
}

void EquipmentObject::ExpansionSkillSlot1()
{
}

void EquipmentObject::ExpansionSkillSlot2()
{
}

void EquipmentObject::ExpansionSkillSlot3()
{
}

void EquipmentObject::ExpansionSkillSlot4()
{
}

void EquipmentObject::ExpansionSkillSlot5()
{
}

void EquipmentObject::ExpansionSkillSlot6()
{
}

void EquipmentObject::SetAnimation(string name, int depth)
{
	_currentAnimator = IMAGEMANAGER.GetAnimator(name);
	_currentAnimator.SetTarget(_currentsprite);
	_depth = depth;
}

void EquipmentObject::SetSlot(slotType slottype)
{
	_slottype = slottype;
}
