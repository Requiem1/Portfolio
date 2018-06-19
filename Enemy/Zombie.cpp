#include "../stdafx.h"
#include "Zombie.h"
#include "../Player/Player.h"
#include "../Map/MHeightMap.h"

Zombie::Zombie():m_state(STATE_MOVE)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_moveSpeed = 00000.1f;
	m_rotSpeed = 8.0f;
	m_bAngleCheck = true;
	//m_pPlayer = Player;
	//SetPlayer();

} 


Zombie::~Zombie()
{
	vecPos.clear();
}

void Zombie::Init()
{
	//this->SetXfile("Resource/Enemy/", "Zombie.x", 0.005f);
	m_pos = D3DXVECTOR3(GetRandomFloat(-30, 30), 0, GetRandomFloat(-30, 30));
	m_vForward = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + 1.0f);
	//m_vForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	DSkinnedMesh::Init();
}

void Zombie::Update()
{
	DSkinnedMesh::Update();

	//m_vPlayerPos = m_pPlayer->GetPosition();
	m_vPlayerPos = g_PlayerMGR->GetMainPlayer()->GetPosition();
	
	m_vUp = D3DXVECTOR3(m_pos.x, m_pos.y+1.0f, m_pos.z);
	m_vLookatPlayer = D3DXVECTOR3(m_vPlayerPos.x - m_pos.x, m_pos.y, m_vPlayerPos.z - m_pos.z);
	
	D3DXVec3Normalize(&m_vForward, &m_vForward);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vLookatPlayer, &m_vLookatPlayer);
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vForward);


	float RotAngle = D3DXVec3Dot(&m_vForward, &m_vLookatPlayer);
	float RotsinAngle = (float)acos(RotAngle);
	

	switch (m_state)
	{
		bool heightcheck;
		case STATE_MOVE:
		{
			// 3.0f라는 거리안에 있는지 체크하여 회전 후 이동할 것
			if (D3DXVec3Length(&(m_vPlayerPos - m_pos)) > 5.0f)
			{
				SetNowAnimation(ZOMBIE_RUN);
				//왼쪽
				if (D3DXVec3Dot(&m_vRight, &m_vLookatPlayer)>0)
				{
					m_rot.y = RotsinAngle;
					m_pos.x += m_vLookatPlayer.x * m_moveSpeed;
					m_pos.z += m_vLookatPlayer.z * m_moveSpeed;
					//heightcheck = g_pCurrentMap->GetHeight(m_pos.y, m_pos);
				}
				//오른쪽
				else
				{
					m_rot.y = -RotsinAngle;
					m_pos.x += m_vLookatPlayer.x * m_moveSpeed;
					m_pos.z += m_vLookatPlayer.z * m_moveSpeed;
					//heightcheck = g_pCurrentMap->GetHeight(m_pos.y, m_pos);
				}

			}
			else
			{
				m_state = STATE_ATTACK;
			}
			break;
		}
		case STATE_ATTACK:
		{
			//공격 범위 안에 있다면
			if (D3DXVec3Length(&(m_vPlayerPos - m_pos)) <= 5.0f)
			{
				SetNowAnimation(ZOMBIE_ATTACK);
				//왼쪽
				if (D3DXVec3Dot(&m_vRight, &m_vLookatPlayer)>0)
				{
					m_rot.y = RotsinAngle;
					//heightcheck = g_pCurrentMap->GetHeight(m_pos.y, m_pos);
				}
				//오른쪽
				else
				{
					m_rot.y = -RotsinAngle;
					//heightcheck = g_pCurrentMap->GetHeight(m_pos.y, m_pos);
				}
			}
			else
			{
				m_state = STATE_MOVE;
			}
		}
		break;

		m_vForward = m_vLookatPlayer;
	}
	//m_BoundingBox->UpdateBoundingBox(m_matWorld, m_pos);
	D3DXMATRIXA16 matS;
	D3DXMATRIXA16 matR;
	D3DXMATRIXA16 matT;

	D3DXMatrixScaling(&matS, m_ScaleXYZ.x, m_ScaleXYZ.y, m_ScaleXYZ.z);
	D3DXMatrixRotationY(&matR, m_rot.y);
	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = matS *matR * matT;
}

void Zombie::Render()
{
	DSkinnedMesh::Render();
}

