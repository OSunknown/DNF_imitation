#pragma once
//TODO:
/*
데이터를 받는다.

그걸로 스테이터스를 생성한다.

생성한 스테이터스와 오브젝트를 가진다.
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
	//플레이어가 가진 케릭터들
	//공용 창고
	//모험단 ?
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
	//유저 데이터를 받아야됨
	//케릭터들 정보
	//선택된 케릭터 정보
	//보여 줄뿐 처리는 서버에서 한다.
	//
	
	vector<CharacterData> slots;
public:
	bool havePlayableCharacter = false;
private:
	BaseCharacterObject* _currentCharacterobject;
public:
	//선택된 케릭터 정보 주세요.
	//getCurrentCharacter();
	BaseCharacterObject* GetCurrentCharacterObject();
	void SetCurrentCharacterObject(int slots);

	void CharacterMove(D3DXVECTOR2 movepoint,bool isRun);

};

