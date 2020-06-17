#include "stdafx.h"
#include "BaseCharacterObject.h"
#include "Animator.h"
#include "EquipmentObject.h"
#include <algorithm>


bool SortCondition(const EquipmentObject& a, const EquipmentObject& b)
{
	if (a._depth < b._depth)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BaseCharacterObject::BaseCharacterObject()
{
	EquipmentChange = false;
}

BaseCharacterObject::~BaseCharacterObject()
{
}

void BaseCharacterObject::Awake()
{
}

void BaseCharacterObject::Start()
{
}

void BaseCharacterObject::Update()
{
}

void BaseCharacterObject::FixedUpdate()
{

}

void BaseCharacterObject::Frame()
{	
	//프레임을 그리기전에 매번 솔팅해줄것?(장비가 교체됬다면..);
	sort(equipEquipment.begin(), equipEquipment.end(), SortCondition);

	for (int i = 0; i < equipEquipment.size(); i++)
	{
		equipEquipment[i].SetPosition(_position);
		equipEquipment[i].Frame();
	}
}

void BaseCharacterObject::Shutdown()
{
}

void BaseCharacterObject::init(int slotNum)
{
	_slotNum = slotNum;
	/*if (GM.slots.size() < (size_t)slotNum)
		return;*/
	// GM 에서 데이터를 받으시오
	CharacterData cdata = GM.slots[slotNum];
	//944b9aab492c15a8474f96947ceeb9e4  직업 ID
	if (cdata.CharacterDatas[(int)characterKeys::jobId] == "944b9aab492c15a8474f96947ceeb9e4 ")
	{
		jobID = "gg";// 여거너
	}

	for (int i = (int)characterKeys::AvataHeadgear; i <= (int)characterKeys::AvataWeapon; i+=2)
	{
		printf("%s \n", cdata.CharacterDatas[i]);
		if (cdata.CharacterDatas[i] == "0")
		{
			//기본 의상 로드. i에 따라서 
			//피부 아바타에 대한 데이터가 없음.
			switch (i)
			{
			case (int)characterKeys::AvataHeadgear:
				break;
			case (int)characterKeys::AvataHair:
				break;
			case (int)characterKeys::AvataFace:
				break;
			case (int)characterKeys::AvataJacket:
				break;
			case (int)characterKeys::AvataShoulder:
				break;
			case (int)characterKeys::AvataPants:
				break;
			case (int)characterKeys::AvataShoes:
				break;
			case (int)characterKeys::AvataWaist:
				break;
			case (int)characterKeys::AvataWeapon:
				break;
			}
		}
		else
		{
			//key에 따라서 값 가져오기
		}
	}
	//string jobID = "gg"; 
	////피부는 기본 피부 만들어준다.
	//string skinID = "gg_body0000_p0";
	//int j = 0;

	//테스트용 스킨
	EquipmentObject equipment;
	equipment.Init();
	equipment.SetAnimation("gg_body0000_p1", 5);
	equipment.Play();
	equipEquipment.push_back(equipment);
}

void BaseCharacterObject::ChangeEquipment(string name)
{

}

void BaseCharacterObject::SetUiType()
{
	for (size_t i = 0; i < equipEquipment.size(); i++)
	{
		equipEquipment[i].SetUiType();
	}
}

void BaseCharacterObject::SetWorldObjectType()
{
	for (size_t i = 0; i < equipEquipment.size(); i++)
	{
		equipEquipment[i].SetWorldObjectType();
	}
}

void BaseCharacterObject::Idle()
{
	for (size_t i = 0; i < equipEquipment.size(); i++)
	{
		equipEquipment[i].Idle();
	}
}

void BaseCharacterObject::Walk()
{
}

void BaseCharacterObject::Run()
{
}
