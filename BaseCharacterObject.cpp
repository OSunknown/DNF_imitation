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
	//�������� �׸������� �Ź� �������ٰ�?(��� ��ü��ٸ�..);
	sort(equipEquipment.begin(), equipEquipment.end(), SortCondition);

	for (int i = 0; i < equipEquipment.size(); i++)
	{
		equipEquipment[i].SetPosition(_position);
		equipEquipment[i].Frame();
	}
}

void BaseCharacterObject::Shutdown()
{
	for (int i = 0; i < equipEquipment.size(); i++)
	{
		equipEquipment[i].Shutdown();
	}
}

void BaseCharacterObject::init(int slotNum)
{
	_slotNum = slotNum;
	/*if (GM.slots.size() < (size_t)slotNum)
		return;*/
	// GM ���� �����͸� �����ÿ�
	CharacterData cdata = GM.slots[slotNum];
	//944b9aab492c15a8474f96947ceeb9e4  ���� ID
	if (cdata.CharacterDatas[(int)characterKeys::jobId] == "944b9aab492c15a8474f96947ceeb9e4 ")
	{
		jobID = "gg";// ���ų�
	}

	for (int i = (int)characterKeys::AvataHeadgear; i <= (int)characterKeys::AvataWeapon; i+=2)
	{
		printf("%s \n", cdata.CharacterDatas[i]);
		if (cdata.CharacterDatas[i] == "0")
		{
			//�⺻ �ǻ� �ε�. i�� ���� 
			//�Ǻ� �ƹ�Ÿ�� ���� �����Ͱ� ����.
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
			//key�� ���� �� ��������
		}
	}
	//string jobID = "gg"; 
	////�Ǻδ� �⺻ �Ǻ� ������ش�.
	//string skinID = "gg_body0000_p0";
	//int j = 0;

	//�׽�Ʈ�� ��Ų
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
	for (size_t i = 0; i < equipEquipment.size(); i++)
	{
		equipEquipment[i].Walk();
	}
}

void BaseCharacterObject::Run()
{
	for (size_t i = 0; i < equipEquipment.size(); i++)
	{
		equipEquipment[i].Run();
	}
}
