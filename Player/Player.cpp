#include "../stdafx.h"
#include "Player.h"
#include "Inventory.h"
#include "../Base/Item.h"
#include "../Map/MHeightMap.h"
#include "../Weapon/GRifle.h"
#include "../Weapon/Bullet.h"

Player::Player() : m_INFO(PlAYERINFO(100, 100, 0, 0, 0))
{
	m_pInventory = NULL;
	m_isMoving = false;
	m_moveSpeed = 0.2f;
	m_currMoveSpeedRate = 1.0f;
	m_rotationSpeed = 0.1f;

	m_BJumping = false;
	m_jumpPower = 1.0f;
	m_gravity = 0.05f;
	m_currGravity = 0.0f;
	m_maxStepHeight = 2.0f;

	m_pos = D3DXVECTOR3(0, 0, 0);
}


Player::~Player()
{
	SAFE_RELEASE(m_Rifle);
}

void Player::Init()
{
	m_Camera = new Camera();
	m_EquipInfo = 0;
	m_pInventory = new Inventory();
	m_pInventory->Init();

	DSkinnedMesh::Init();
	DSkinnedMesh::Update();

	g_INPUTMGR->SetPosition(&m_DeltaPos, &m_DeltaRot, &m_BJumping);
	m_Camera->SetTarget(&m_pos);
	m_Camera->Init();
	m_Camera->Setdistance(true);

	g_DisplayObjMGR->AddObjectWithTag(this, PLAYER_TAG);
	m_BoundingBox = new CBox;
	m_BoundingBox->initBoundingBox(NULL, D3DXVECTOR3(1.0f, 3.5f, 1.0f), m_pos);

	// 확인을 위해 시작시에 애니메이션 지정 - 재익 수정
	SetNowAnimation_Up(PLAYER_MOVE);
	SetNowAnimation_Down(PLAYER_STAND);

	// Sound의 리스너 설정
	g_SoundMGR->SetListener(INDEX_PLAYER_1, &m_pos, &m_forward, &Upvec);


	m_Rifle = new GRifle();
	m_Rifle->LoadMesh("Resource/Gun", "M16.obj", 0.4f);
	m_Rifle->Init();

	m_Rifle->SetParantWM(&m_HandFrame_L);
	m_Rifle->SetParantWM2(&m_HandFrame_R);
	m_Rifle->SetType(SHOT);

}

void Player::Update()
{
	// 애니메이션 시간 보여주는 함수 - 재익 수정
	DebugAnimationTime();
	

	Upvec = D3DXVECTOR3(0, 1, 0);
	D3DXVec3Cross(&Right, &Upvec, &m_forward);
	D3DXVec3Cross(&Upvec, &m_forward, &Upvec);
	D3DXVec3Normalize(&Upvec, &Upvec);
	Left = D3DXVECTOR3(-Right.x, 0, -Right.z);


	if (m_DeltaPos.z == 1 || m_DeltaPos.z == -1)
	{
		SetNowAnimation_Down(PLAYER_MOVE);
	}
	else
	{
		SetNowAnimation_Down(PLAYER_STAND);
	}
	m_rotY = m_Camera->GetRotY();
	D3DXMATRIXA16   m_matRotX, m_matRotY;
	if (m_rotY <=5.5f || m_rotY >=7.5f)
	{
		m_isDownBodyMove = true;
		D3DXMatrixRotationY(&m_matRotY, m_rotY);
		D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0, 0, 1), &m_matRotY);
	}
	else
	{
		m_isDownBodyMove = false;
		m_rot.y = m_rotY;
		D3DXMatrixRotationY(&m_matRotY, m_rotY);
		D3DXVec3TransformNormal(&UpBodyForward, &D3DXVECTOR3(0, 0, 1), &m_matRotY);
	}

	D3DXVECTOR3 targetPos;

	bool	isIntersected = true;
	float	height = 0;

	if (m_BJumping == true)
	{
		m_currMoveSpeedRate = 0.7f;
		if (m_DeltaRot.y == -1)
		{
			targetPos = m_pos + Left * 1.0f
				* m_moveSpeed * m_currMoveSpeedRate;
		}
		else if (m_DeltaRot.y == 1)
		{
			targetPos = m_pos + Right * 1.0f
				* m_moveSpeed * m_currMoveSpeedRate;
		}
		else
		{

			targetPos = m_pos + m_forward * m_DeltaPos.z
				* m_moveSpeed * m_currMoveSpeedRate;
		}

		targetPos.y += m_jumpPower - m_currGravity;
		m_currGravity += m_gravity;

		if (g_pCurrentMap != NULL)
		{
			isIntersected = g_pCurrentMap->GetHeight(height, targetPos);
		}
		if (isIntersected == false)
		{
			if (g_pCurrentMap != NULL)
			{
				isIntersected = g_pCurrentMap->GetHeight(height, m_pos);
			}
			m_pos.y = targetPos.y;
		}
		else
		{
			m_pos = targetPos;
		}



		if (m_pos.y <= height && m_jumpPower < m_currGravity)
		{
			m_pos.y = height;
			m_BJumping = false;
			m_currGravity = 0;
			m_currMoveSpeedRate = 1.0f;
		}

		//m_pos = targetPos;
	}
	else //m_IsJumping == false
	{
		if (m_DeltaRot.y == -1)
		{
			targetPos = m_pos + Left * 1.0f
				* m_moveSpeed * m_currMoveSpeedRate;

		}
		else if (m_DeltaRot.y == 1)
		{
			targetPos = m_pos + Right * 1.0f
				* m_moveSpeed * m_currMoveSpeedRate;
		}
		else
		{
			targetPos = m_pos + m_forward * m_DeltaPos.z
				* m_moveSpeed * m_currMoveSpeedRate;

		}

		if (g_pCurrentMap != NULL)
		{
			isIntersected = g_pCurrentMap->GetHeight(height, targetPos);
			int tmp = 0;
			if (isIntersected == false || fabs(height - m_pos.y) > m_maxStepHeight)
			{

			}
			else
			{
				targetPos.y = height;
				m_pos = targetPos;
			}
		}
		else
		{
			m_pos = targetPos;

		}
	}

	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);

	D3DXMATRIXA16 matBaseR;
	D3DXMatrixRotationY(&matBaseR, D3DX_PI);


	// 상체의 회전을 위해 m_matRotY을 초기화
	//D3DXMatrixIdentity(&m_matRotY);
	if (m_isDownBodyMove)
	{
		m_matWorld = matBaseR * m_matRotY * matT;
	}
	else
	{
		D3DXMatrixIdentity(&m_matRotY);
		m_matWorld = matBaseR * m_matRotY * matT;
	}

	if (D3DXVec3LengthSq(&m_DeltaRot) > D3DX_16F_EPSILON ||
		D3DXVec3LengthSq(&m_DeltaPos) > D3DX_16F_EPSILON)
		m_isMoving = true;
	else
		m_isMoving = false;

	m_Camera->Update();

	/*
	for (int i = 0; i < m_vecBullet.size(); i++)
		m_vecBullet[i]->Update();
	*/

	DSkinnedMesh::Update();

	m_BoundingBox->UpdateBoundingBox(m_matWorld);
	if (g_INPUTMGR->ButtonDown(g_INPUTMGR->LBUTTON))
	{
		m_Rifle->SetClick(true);
		m_Rifle->SetForward(UpBodyForward);
	}
	else
		m_Rifle->SetClick(false);
	m_Rifle->Update();
}

void Player::Render()
{
	m_Rifle->Render();
	m_BoundingBox->RenderBoundingBox();
	DSkinnedMesh::Render();

	// 확인용
	Debug->AddText("플레이어 위치 : ");
	Debug->AddText(m_pos);
	Debug->EndLine();
}


void Player::Equip()
{
	if (m_bHead)
		m_EquipInfo |= ITEMEQUIP::HEAD;
	else
		m_EquipInfo ^= ITEMEQUIP::HEAD;

	if (m_bBody)
		m_EquipInfo |= ITEMEQUIP::BODY;
	else
		m_EquipInfo ^= ITEMEQUIP::BODY;

	if (m_bReg)
		m_EquipInfo |= ITEMEQUIP::REG;
	else
		m_EquipInfo ^= ITEMEQUIP::REG;
}

void Player::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_Camera->WndProc(hWnd, message, wParam, lParam);
}


