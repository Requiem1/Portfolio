#pragma once
#include "../Base/IAnimationObject.h"


class Inventory;
class DITEM;
class GRifle;
// class ��

// ������ �������¸� �������� �������̸� ��Ʈ����ũ�� ó���Ѵ�.
enum ITEMEQUIP
{
	HEAD = 1,
	BODY = 2,
	REG  = 4
};

// �÷��̾��� �⺻ �������̸� UI���� Ȯ���Ͽ� ������ ������.
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
   ��ü �ִϸ��̼�
   Spine01
*/

// �� Ŭ������ �÷��̾��� �ʱ�ȭ, �׸���, ������ ����Ѵ�.
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

	// 0622 ���� �ۼ�
	GRifle *		m_Rifle;

public:
	Player();
	~Player();

	// IAnimationObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;



	// ���� ĳ���Ͱ� � ��� �����ߴ��� �������ش�.
	void SetEquip(bool Head, bool Body, bool reg)
	{
		m_bHead = Head;
		m_bBody = Body;
		m_bReg = reg;
	}

	// ��� ���� ���¸� ��ȯ�Ѵ�.
	bool &   GetBoolHead()
	{
		return m_bHead;
	}
	
	// ��ź��(��)�� ���� ���¸� ��ȯ�Ѵ�.
	bool &  GetBoolBody()
	{
		return m_bBody;
	}
	// ����(��)�� ���� ���¸� ��ȯ�Ѵ�.
	bool & GetBoolReg()
	{
		return m_bReg;
	}
	
	// SetEquip()�Լ����� �������� bool ������ Ȯ���Ͽ� ��Ʈ����ũ�� ��� ��Ȳ�� �����Ѵ�.
	void Equip();




	// �÷��̾��� ���� �κ��丮 ��Ȳ�� �����Ѵ�.
	Inventory* GetInveontory()
	{
		return m_pInventory;
	}
	// �÷��̾��� ���¸� �����Ѵ�.
	void SetPlayerInfo(int _HP)
	{
		m_INFO.HP = _HP;
	}
	// ���� �÷��̾��� ���¸� �����´�.
	PlAYERINFO  GetPlayerInfo()
	{
		return m_INFO;
	}

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

