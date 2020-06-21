#pragma once
//TODO:
/*
�����͸� �޴´�.

�װɷ� �������ͽ��� �����Ѵ�.

������ �������ͽ��� ������Ʈ�� ������.
*/

enum class characterKeys
{
	characterId = 0,
	characterName,
	characterLevel,
	jobId,
	jobName,
	jobGrowId,
	jobGrowName,
	adventureName,
	guildId,
	guildName,
	Weapon,
	WeaponGrade,
	Jacket,
	JacketGrade,
	Shoulder,
	ShoulderGrade,
	Pants,
	PantsGrade,
	Shoes,
	ShoesGrade,
	Waist,
	WaistGrade,
	AvataHeadgear,
	AvataHeadgearOptionAbility,
	AvataHair,
	AvataHairOptionAbility,
	AvataFace,
	AvataFaceOptionAbility,
	AvataJacket,
	AvataJacketOptionAbility,
	AvataShoulder,
	AvataShoulderOptionAbility,
	AvataPants,
	AvataPantsOptionAbility,
	AvataShoes,
	AvataShoesOptionAbility,
	AvataWaist,
	AvataWaistOptionAbility,
	AvataWeapon,
	AvataWeaponOptionAbility

};
struct Equipment
{
	string slotId;
	string slotName;
	string itemId;
	string itemName;
	string itemType;
	string itemTypeDetail;
	int  itemAvailableLevel;
	string itemRarity;
	string setItemId;
	string setItemName;
	int reinforce;
	string itemGradeName;
	string amplificationName;
	int refine;
};

struct AvataData
{
	string slotId;
	string slotName;
	string itemId;
	string itemName;
	string itemRarity;
};
struct CharacterData
{
	vector<char*> CharacterDatas;
	vector<char*> EquipmentDatas;
	vector<char*> AvataDatas;
};
struct userData
{
	//�÷��̾ ���� �ɸ��͵�
	//���� â��
	//����� ?
};
class GameMaster
{
public:
	GameMaster();
	~GameMaster();
	static GameMaster & inse() { static GameMaster instance; return instance; }
public:
	void Init();
	int SetCharacterData(char* data);
public:
	//���� �����͸� �޾ƾߵ�
	//�ɸ��͵� ����
	//���õ� �ɸ��� ����
	//���� �ٻ� ó���� �������� �Ѵ�.
	//
	
	vector<CharacterData> slots;
public:
	bool havePlayableCharacter = false;
private:
	BaseCharacterObject* _currentCharacterobject;
public:
	//���õ� �ɸ��� ���� �ּ���.
	//getCurrentCharacter();
	BaseCharacterObject* GetCurrentCharacterObject();
	void SetCurrentCharacterObject(int slots);

	void CharacterMove(D3DXVECTOR2 movepoint,bool isRun);
	void CharacterFilp(bool Filp);

};

