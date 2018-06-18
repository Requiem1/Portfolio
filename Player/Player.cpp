#include "../stdafx.h"
#include "Player.h"
#include "Inventory.h"
#include "../Base/Item.h"
#include "../Map/MHeightMap.h"


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


}


Player::~Player()
{
	//SAFE_DELETE(m_pInventory);
	//SAFE_RELEASE(TestGrid);

	/*

	for(auto p : m_vecBullet)
		SAFE_RELEASE(p);
	*/
}

void Player::Init()
{
	// ���� BGM�� ���� ������ �ȵǴµ� �����Ƽ� �� ����
	/*

	g_SoundMGR->AddSound(L"BGM", "Resource/Sound/BGM/ArgentCombat.mp3", true, true);

	g_SoundMGR->AddSound(L"Rifle", "Resource/Sound/SE/ak74-fire.wav", false, false);
	g_SoundMGR->AddSound(L"Shotgun", "Resource/Sound/SE/spas12-fire.wav", false, false);
	g_SoundMGR->AddSound(L"GunChange", "Resource/Sound/SE/spas12-reload.wav", false, false);

	g_SoundMGR->Play(L"BGM", true);
	*/
	m_Camera = new Camera();

	m_EquipInfo = 0;
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_pInventory = new Inventory();
	m_pInventory->Init();

	DSkinnedMesh::Init();
	//TestGrid = new Grid();
	//TestGrid->Init();
	//PrintBoneList();


	DSkinnedMesh::Update();

	g_INPUTMGR->SetPosition(&m_DeltaPos, &m_DeltaRot, &m_BJumping);
	m_Camera->SetTarget(&m_pos);
	m_Camera->Init();
	m_Camera->Setdistance(true);
	m_Camera->SetCharacterForword(&m_matWorld);

}

void Player::Update()
{
	//Equip();
	//m_pInventory->Update();


	m_rot += m_DeltaRot * m_rotationSpeed;


	Upvec = D3DXVECTOR3(0, 1, 0);
	D3DXVec3Cross(&Right, &Upvec, &m_forward);
	D3DXVec3Cross(&Upvec, &m_forward, &Upvec);
	D3DXVec3Normalize(&Upvec, &Upvec);
	Left = D3DXVECTOR3(-Right.x, 0, -Right.z);


	if (m_DeltaPos.z == 1 || m_DeltaPos.z == -1)
	{
		SetNowAnimation(PLAYER_MOVE);
	}
	else
	{
		SetNowAnimation(PLAYER_STAND);
	}

	m_rotY = m_Camera->GetRotY();

	D3DXMATRIXA16   m_matRotX, m_matRotY;
	D3DXMatrixRotationY(&m_matRotY, m_rotY);
	D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0, 0, 1), &m_matRotY);
	


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
		//m_pos = targetPos;
	}

	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);

	D3DXMATRIXA16 matBaseR;
	D3DXMatrixRotationY(&matBaseR, D3DX_PI);


	m_matWorld = matBaseR * m_matRotY* matT;


	if (D3DXVec3LengthSq(&m_DeltaRot) > D3DX_16F_EPSILON ||
		D3DXVec3LengthSq(&m_DeltaPos) > D3DX_16F_EPSILON)
		m_isMoving = true;
	else
		m_isMoving = false;


	DSkinnedMesh::Update();

	m_Camera->Update();
	/*

	for (int i = 0; i < m_vecBullet.size(); i++)
		m_vecBullet[i]->Update();
	*/
}

void Player::Render()
{
	DSkinnedMesh::Render();
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


