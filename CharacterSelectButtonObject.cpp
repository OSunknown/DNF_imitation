#include "stdafx.h"
#include "CharacterSelectButtonObject.h"


CharacterSelectButtonObject::CharacterSelectButtonObject()
{
	slotCharacter = NULL;
	slotSelectImage = NULL;
	_slotNumber = -1;
	mouseX = 0;
	mouseY = 0;
	_width = 0;
	_height = 0;
}

CharacterSelectButtonObject::~CharacterSelectButtonObject()
{
}

void CharacterSelectButtonObject::Init(int slotNumber, float PosX, float PosY)
{
	slotSelectImage = new ImageObject();
	_slotNumber = slotNumber;
	SetTransform();
	_position.x = PosX;
	_position.y = PosY;

	slotSelectImage->_currentsprite = IMAGEMANAGER.GetUISprite("SelectUnit");
	slotSelectImage->Init();
	slotSelectImage->SetPosition(D3DXVECTOR3(_position.x, _position.y, 0));

	_width = slotSelectImage->_currentsprite->width;
	_height = slotSelectImage->_currentsprite->height;

	if (GM.slots.size() > _slotNumber)
	{
		slotCharacter = new BaseCharacterObject();
		slotCharacter->init(slotNumber);
		slotCharacter->SetUiType();
		slotCharacter->SetPosition(_position + D3DXVECTOR2(0, -50));
		MS.AddUIObject(slotCharacter);
	}
}

void CharacterSelectButtonObject::Init(int slotNumber, float PosX, float PosY, float width, float height)
{
	slotSelectImage = new ImageObject();
	_slotNumber = slotNumber;
	SetTransform();
	_position.x = PosX;
	_position.y = PosY;
	_width = width;
	_height = height;

	slotSelectImage->_currentsprite = IMAGEMANAGER.GetUISprite("SelectUnit");
	slotSelectImage->Init();
	slotSelectImage->SetPosition(D3DXVECTOR3(_position.x, _position.y,0));
	if (GM.slots.size() > _slotNumber)
	{
		slotCharacter = new BaseCharacterObject();
		slotCharacter->init(slotNumber);
		slotCharacter->SetUiType();
		slotCharacter->SetPosition(_position + D3DXVECTOR2(0, -50));
		//MS.AddUIObject(slotCharacter);
	}
	
}

void CharacterSelectButtonObject::Awake()
{

}

void CharacterSelectButtonObject::Start()
{
}

void CharacterSelectButtonObject::Update()
{
}

void CharacterSelectButtonObject::FixedUpdate()
{
}

void CharacterSelectButtonObject::Frame()
{
	Input.ScreenToWorldPoint(mouseX, mouseY);
	if (slotCharacter != nullptr)
	{
		slotCharacter->Frame();
	}
		
	slotSelectImage->SetRect();

	//마우스 위치가 범위 내에 있는지 확인해서 슬롯 정보에 대해 반환하기
	if (slotSelectImage->left <= (float)mouseX && (float)mouseX <= slotSelectImage->right)
	{
		if (slotSelectImage->bottom <= (float)mouseY && (float)mouseY <= slotSelectImage->top)
		{
			//printf("1 : %f %f %f %f \n", left, right, top, bottom);
			slotSelectImage->Render();
			if (Input.GetMouseButtonDown(0))
			{
				if (btnOK == false)
				{
					btnOK = true;
					//버튼 클릭했을때 이벤트
					if (slotCharacter != nullptr)
					{
						printf("OK %d \n", _slotNumber);
						//TODO:게임 마스터에게 current 케릭터를 가지고 있게 한다.
						//
						GM.SetCurrentCharacterObject(_slotNumber);
						MS.UIClear();
						MS.ChangeScene(sceneNames::Scene_Elvenguard);
					}
				}				
			}
			else
			{
				if (btnOK == true)
				{
					btnOK = false;
				}
			}
		}
	}
	

	
}

void CharacterSelectButtonObject::Shutdown()
{
	if (slotSelectImage != NULL)
	{
		slotSelectImage->Shutdown();
		delete slotSelectImage;
	}

	if (slotCharacter != NULL)
	{
		slotCharacter->Shutdown();
		delete slotCharacter;
	}
}
