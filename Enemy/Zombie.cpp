#include "../stdafx.h"
#include "Zombie.h"
#include "../Player/Player.h"
#include "../Map/MHeightMap.h"

Zombie::Zombie() :m_state(STATE_MOVE)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_moveSpeed = 0.4f;
	m_rotSpeed = 0.1f;
	m_bAngleCheck = true;
}


Zombie::~Zombie()
{
}

void Zombie::Init()
{
	m_pos = D3DXVECTOR3(GetRandomFloat(-30, 30), 15, GetRandomFloat(-30, 30));
	//m_vForward = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + 1.0f);
	m_vForward = D3DXVECTOR3(0, 0, 1);
	DSkinnedMesh::Init();

	m_BoundingBox->initBoundingBox(NULL, D3DXVECTOR3(1.0f, 3.5f, 1.0f), m_pos);
	g_DisplayObjMGR->AddObjectWithTag(this, ENEMY_TAG);
}

void Zombie::Update()
{
	DSkinnedMesh::Update();

	m_vPlayerPos = g_PlayerMGR->GetMainPlayer()->GetPosition();

	m_vUp = D3DXVECTOR3(m_pos.x, m_pos.y + 1.0f, m_pos.z);
	m_vLookatPlayer = m_vPlayerPos - m_pos;
	//m_vLookatPlayer = D3DXVECTOR3(m_vPlayerPos.x - m_pos.x, m_pos.y, m_vPlayerPos.z - m_pos.z);

	D3DXVec3Normalize(&m_vForward, &m_vForward);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vLookatPlayer, &m_vLookatPlayer);
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vForward);


	float RotAngle = D3DXVec3Dot(&m_vForward, &m_vLookatPlayer);
	float RotsinAngle = (float)acos(RotAngle);
	bool heightcheck = g_pCurrentMap->GetHeight(m_pos.y, m_pos);

	switch (m_state)
	{
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
					//m_pos += m_vLookatPlayer*m_moveSpeed*0.5f;
					//heightcheck = g_pCurrentMap->GetHeight(m_pos.y, m_pos);
				}
				//오른쪽
				else
				{
					m_rot.y = -RotsinAngle;

					m_pos.x += m_vLookatPlayer.x * m_moveSpeed;
					m_pos.z += m_vLookatPlayer.z * m_moveSpeed;
					//m_pos += m_vLookatPlayer*m_moveSpeed*0.5f;
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

				// 왼쪽 / 오른쪽
				if (D3DXVec3Dot(&m_vRight, &m_vLookatPlayer) > 0)
				{
					m_rot.y = RotsinAngle;
				}
				else
				{
					m_rot.y = -RotsinAngle;
				}
			}
			else
			{
				m_state = STATE_MOVE;
			}
			break;
		}

	}

	D3DXMATRIXA16 matS;
	D3DXMATRIXA16 matR;
	D3DXMATRIXA16 matT;

	D3DXMatrixScaling(&matS, m_ScaleXYZ.x, m_ScaleXYZ.y, m_ScaleXYZ.z);
	D3DXMatrixRotationY(&matR, m_rot.y + D3DX_PI);		// BaseR 부분을 m_rot에 같이 계산함
	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);

	m_matWorld = matS * matR * matT;

	// 좀비 충돌 계산
	vector<IDisplayObject*> Dis;
	Dis = g_DisplayObjMGR->CollideCheckWithTag_ReturnVecFunc(this, 1, ENEMY_TAG);

	if (Dis.size() > 0)
	{
		Debug->AddText("좀비 충돌중 -> size : ");
		Debug->AddText((int)Dis.size());
		Debug->EndLine();
	}


	// 좀비는 Scail이 적용된 상태지만 바운딩박스에는 스케일이 들어가면 안되므로
	// matS를 제외한 matR과 matT로 한다
	D3DXMATRIXA16 matRT = matR * matT;
	m_BoundingBox->UpdateBoundingBox(matRT, 3);
}

void Zombie::Render()
{
	DSkinnedMesh::Render();
	m_BoundingBox->RenderBoundingBox();
}

 