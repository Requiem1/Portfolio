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

	m_IsJumping = false;
	m_jumpPower = 1.0f;
	m_gravity = 0.05f;
	m_currGravity = 0.0f;
	m_maxStepHeight = 2.0f;
}


Player::~Player()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	//SAFE_DELETE(m_pInventory);
	//SAFE_RELEASE(TestGrid);

	for(auto p : m_vecBullet)
		SAFE_RELEASE(p);
}

void Player::Init()
{
	// 원래 BGM은 여기 넣으면 안되는데 귀찬아서 걍 넣음
	g_SoundMGR->AddSound(L"BGM", "Resource/Sound/BGM/ArgentCombat.mp3", true, true);

	g_SoundMGR->AddSound(L"Rifle", "Resource/Sound/SE/ak74-fire.wav", false, false);
	g_SoundMGR->AddSound(L"Shotgun", "Resource/Sound/SE/spas12-fire.wav", false, false);
	g_SoundMGR->AddSound(L"GunChange", "Resource/Sound/SE/spas12-reload.wav", false, false);

	g_SoundMGR->Play(L"BGM", true);

	m_EquipInfo = 0;
	m_pos = D3DXVECTOR3(0, 1, 0);
	m_pInventory = new Inventory();
	m_pInventory->Init();
	//TestGrid = new Grid();
	//TestGrid->Init();
	vector<D3DXVECTOR3> vecPos;
	for (size_t i = 0; i < CUBE_VERTEX_SIZE; i++)
	{
		vecPos.push_back(g_aCubeVertex[i]);
	}

	// 버텍스 설정
	D3DCOLOR red = D3DCOLOR_XRGB(255, 0, 0);
	D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);
	D3DCOLOR blue = D3DCOLOR_XRGB(0, 0, 255);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);
	D3DCOLOR yellow = D3DCOLOR_XRGB(255, 255, 0);

	m_vecVertex.push_back(VERTEX_PC(vecPos[0], white));
	m_vecVertex.push_back(VERTEX_PC(vecPos[1], yellow));
	m_vecVertex.push_back(VERTEX_PC(vecPos[2], green));
	m_vecVertex.push_back(VERTEX_PC(vecPos[3], blue));
	m_vecVertex.push_back(VERTEX_PC(vecPos[4], white));
	m_vecVertex.push_back(VERTEX_PC(vecPos[5], yellow));
	m_vecVertex.push_back(VERTEX_PC(vecPos[6], red));
	m_vecVertex.push_back(VERTEX_PC(vecPos[7], green));

	for (int i = 0; i < CUBE_INDEX_SIZE; i++)
	{
		m_vecIndex.push_back(g_aCubeIndex[i]);
	}
	VertexBuffer(m_pVB, m_pIB, m_vecVertex, m_vecIndex);

	m_pVB->GetDesc(&m_VBDesc);
	m_pIB->GetDesc(&m_IBDesc);
	
	g_INPUTMGR->SetPosition(&m_DeltaPos, &m_DeltaRot, &m_IsJumping);
	
	g_pCamera->SetTarget(&m_pos);

}

void Player::Update()
{
	//Equip();
	m_pInventory->Update();

	m_rot += m_DeltaRot * m_rotationSpeed;

	D3DXVECTOR3 Right, Left;
	D3DXVECTOR3 Upvec = g_pCamera->GetCameraUp();
	D3DXVec3Cross(&Right, &Upvec, &m_forward);
	Left = D3DXVECTOR3(-Right.x, 0, -Right.z);


	D3DXMATRIXA16 matRot;
	matRot = g_pCamera->GetRot();
	D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0, 0, 1), &matRot);


	D3DXVECTOR3 targetPos;

	bool	isIntersected = true;
	float	height = 0;

	if (m_IsJumping == true)
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
			m_IsJumping = false;
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
	if (GetKeyState('E') & 0x8000)
		g_SoundMGR->Play(L"GunChange", false);

	if (GetKeyState('E') & 0x0001)
	{
		if (g_INPUTMGR->ButtonDown(g_INPUTMGR->LBUTTON))
		{
			for (int i = 0; i < 5; i++)
			{
				DITEM * item;
				D3DXVECTOR3 * tempDirection;
				tempDirection = new D3DXVECTOR3();
				DiretcionDecide(tempDirection);
				item = new DITEM();
				item->Init();
				item->SetClick(true);
				item->SetBullet(tempDirection, &m_pos);
				m_vecBullet.push_back(item);
			}

			g_SoundMGR->Play(L"Shotgun", false);
		}
		
	}
	else
	{
		if (g_INPUTMGR->ButtonDown(g_INPUTMGR->LBUTTON))
		{
			DITEM * item;
			item = new DITEM();
			item->Init();
			item->SetClick(true);
			item->SetBullet(&m_forward, &m_pos);
			m_vecBullet.push_back(item);
			g_SoundMGR->Play(L"Rifle", false);
		}

	}

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = matRot * matT;

	if (D3DXVec3LengthSq(&m_DeltaRot) > D3DX_16F_EPSILON ||
		D3DXVec3LengthSq(&m_DeltaPos) > D3DX_16F_EPSILON)
		m_isMoving = true;
	else
		m_isMoving = false;

	for (int i = 0; i < m_vecBullet.size(); i++)
		m_vecBullet[i]->Update();
}

void Player::Render()
{
	//TestGrid->Render();
	g_Device->SetRenderState(D3DRS_LIGHTING, false);
	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_Device->SetFVF(VERTEX_PC::FVF);
	g_Device->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
	g_Device->SetIndices(m_pIB);
	g_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_VBDesc.Size, 0, m_IBDesc.Size / 3);
	for (int i = 0; i < m_vecBullet.size(); i++)
		m_vecBullet[i]->Render();

}

void Player::VertexBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex)
{
	
	g_Device->CreateVertexBuffer(vecVertex.size() * sizeof(VERTEX_PC),
		0, VERTEX_PC::FVF, D3DPOOL_MANAGED, &pVb, NULL);

	VERTEX_PC* pVertex;
	pVb->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, &vecVertex[0], vecVertex.size() * sizeof(VERTEX_PC));
	pVb->Unlock();
	vecVertex.clear();

	g_Device->CreateIndexBuffer(vecIndex.size() * sizeof(WORD),
		NULL, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIb, NULL);

	WORD* pIndex;
	pIb->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &vecIndex[0], vecIndex.size() * sizeof(WORD));
	pIb->Unlock();


	vecIndex.clear();
	

	
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

void Player::DiretcionDecide(OUT D3DXVECTOR3 * Direction)
{
	D3DXVECTOR3 Change = m_forward;
	D3DXMATRIXA16 XRot, YRot, Mat;

	float XRadian, YRadian;
	XRadian = GetRandomFloat(-0.3, 0.3);
	YRadian = GetRandomFloat(-0.3, 0.3);


	D3DXMatrixRotationX(&XRot, XRadian);
	D3DXMatrixRotationY(&YRot, YRadian);

	Mat = XRot + YRot;

	D3DXVec3TransformNormal(Direction, &Change, &Mat);
}
