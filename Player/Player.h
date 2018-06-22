#pragma once
#include "../Base/IAnimationObject.h"


class Inventory;
class DITEM;
class GRifle;
// class 벽

// 아이템 장착상태를 변경해줄 열거형이며 비트마스크로 처리한다.
enum ITEMEQUIP
{
	HEAD = 1,
	BODY = 2,
	REG  = 4
};

// 플레이어의 기본 데이터이며 UI에서 확인하여 보여줄 열거형.
struct PlAYERINFO
{
	int HP;
	int SP;
	int BodyDEF;
	int HeadDEF;
	int RegDEF;

	PlAYERINFO(int H, int S, int B, int Head, int R) :HP(H), SP(S), BodyDEF(B), HeadDEF(Head), RegDEF(R) {}

};

/*
   상체 애니메이션
   Spine01
*/

// 이 클래스는 플레이어의 초기화, 그리기, 갱신을 담당한다.
class Player :public IAnimationObject
{
private:
	//Grid*                   TestGrid;
	PlAYERINFO              m_INFO;
	int                     m_EquipInfo;

	LPD3DXMESH              m_Mesh;

	//vector<DITEM*>           m_vecBullet;

	Camera         * m_Camera;
	D3DXVECTOR3		m_DeltaPos;
	D3DXVECTOR3		m_DeltaRot;
	D3DXVECTOR3		m_forward;
	D3DXVECTOR3     m_HeadPos;
	D3DXMATRIXA16   matT;
	D3DXVECTOR3     Right, Left;
	D3DXVECTOR3     Upvec;

	bool			m_isMoving;
	float			m_moveSpeed;
	float			m_currMoveSpeedRate;
	float			m_rotationSpeed;

	float           m_rotY;
	bool			m_BJumping;
	float			m_jumpPower;
	float			m_gravity;
	float			m_currGravity;

	

	float			m_maxStepHeight;

	Inventory *     m_pInventory;

	bool            m_bHead;
	bool            m_bBody;
	bool            m_bReg;

	// 0622 재익 작성
	GRifle *		m_Rifle;

public:
	Player();
	~Player();

	// IAnimationObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;



	// 현재 캐릭터가 어떤 장비를 장착했는지 세팅해준다.
	void SetEquip(bool Head, bool Body, bool reg)
	{
		m_bHead = Head;
		m_bBody = Body;
		m_bReg = reg;
	}

	// 헬멧 착용 상태를 반환한다.
	bool &   GetBoolHead()
	{
		return m_bHead;
	}
	
	// 방탄복(등)의 착용 상태를 반환한다.
	bool &  GetBoolBody()
	{
		return m_bBody;
	}
	// 바지(등)의 착용 상태를 반환한다.
	bool & GetBoolReg()
	{
		return m_bReg;
	}
	
	// SetEquip()함수에서 셋팅해준 bool 변수를 확인하여 비트마스크로 장비 현황을 셋팅한다.
	void Equip();




	// 플레이어의 현재 인벤토리 현황을 리턴한다.
	Inventory* GetInveontory()
	{
		return m_pInventory;
	}
	// 플레이어의 상태를 갱신한다.
	void SetPlayerInfo(int _HP)
	{
		m_INFO.HP = _HP;
	}
	// 현재 플레이어의 상태를 가져온다.
	PlAYERINFO  GetPlayerInfo()
	{
		return m_INFO;
	}

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

