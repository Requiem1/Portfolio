#include "../stdafx.h"
#include "Zombie.h"
#include "../Player/Player.h"
#include "../Map/MHeightMap.h"

Zombie::Zombie():m_state(STATE_MOVE)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_pVB = NULL;
	m_pIB = NULL;
	m_moveSpeed = 00000.1f;
} 


Zombie::~Zombie()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	vecPos.clear();
}

void Zombie::Init()
{
	for (size_t i = 0; i < CUBE_VERTEX_SIZE; i++)
	{
		vecPos.push_back(g_aCubeVertex[i]);
	}
	
	// 상자의 한 변은 2 * 2
	// radius는 1.5 정도로 하자

	SetVertex(m_vecVertex, m_vecIndex, vecPos, white);
	CreateBuffer(m_pVB, m_pIB, m_vecVertex, m_vecIndex);

	m_pVB->GetDesc(&m_VBDesc);
	m_pIB->GetDesc(&m_IBDesc);


	m_pos = D3DXVECTOR3(GetRandomFloat(-30, 30), 1, GetRandomFloat(-30, 30));
	m_pPlayer = g_PlayerMGR->GetPlayer();
	BoundingBox->initBoundingBox(NULL);
	g_DisplayObjMGR->AddObjectWithTag(this, ENEMY_TAG);


}

void Zombie::Update()
{
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = matT;	//matS * matT;
	bool heightcheck;
	m_Destination = m_pPlayer[0]->GetPosition();



	// ENUM 으로 정의된 m_state 값을 통해 FSM 실행
	// m_Destination 변수에 플레이어의 포지션을 받아 두고
	// 플레이어 포지션 - 좀비들의 포지션 의 길이를 비교하여 
	//3.0f 이상일경우 STATE_MOVE 상태를 유지 하고
	//     이하일경우에 STATE_ATTACK 상태로 변이 한다.
	switch (m_state)
	{
		case STATE_MOVE:
		{
			if (D3DXVec3Length(&(m_Destination - m_pos)) > 3.0f)
			{
				D3DXVec3Normalize(&m_Destination, (&(m_Destination - m_pos)));
				m_pos.x += m_Destination.x *m_moveSpeed;
				heightcheck = g_pCurrentMap->GetHeight(m_pos.y, m_pos);
				m_pos.z += m_Destination.z * m_moveSpeed;
			}
			else
			{
				SetVertex(m_vecVertex, m_vecIndex, vecPos, red);
				SetBuffer(m_pVB, m_pIB, m_vecVertex, m_vecIndex);
				m_state = STATE_ATTACK;
			}
			break;
		}
		case STATE_ATTACK:
		{
			if (D3DXVec3Length(&(m_Destination - m_pos)) <= 3.0f)
			{
				heightcheck = g_pCurrentMap->GetHeight(m_pos.y, m_pos);

			}
			else
			{
				SetVertex(m_vecVertex, m_vecIndex, vecPos, white);
				SetBuffer(m_pVB, m_pIB, m_vecVertex, m_vecIndex);
				m_state = STATE_MOVE;
			}
		}
		break;
	}

	BoundingBox->UpdateBoundingBox(m_matWorld, m_pos);
}

void Zombie::Render()
{
	g_Device->SetRenderState(D3DRS_LIGHTING, false);
	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_Device->SetFVF(VERTEX_PC::FVF);
	g_Device->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
	g_Device->SetIndices(m_pIB);
	g_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_VBDesc.Size, 0, m_IBDesc.Size / 3);

	BoundingBox->RenderBoundingBox();
}


void Zombie::SetVertex(vector<VERTEX_PC>& vecVertexOut, vector<WORD>& vecIndexOut, vector<D3DXVECTOR3> vecPos, D3DCOLOR color)
{
	vecVertexOut.push_back(VERTEX_PC(vecPos[0], color));
	vecVertexOut.push_back(VERTEX_PC(vecPos[1], color));
	vecVertexOut.push_back(VERTEX_PC(vecPos[2], color));
	vecVertexOut.push_back(VERTEX_PC(vecPos[3], color));
	vecVertexOut.push_back(VERTEX_PC(vecPos[4], color));
	vecVertexOut.push_back(VERTEX_PC(vecPos[5], color));
	vecVertexOut.push_back(VERTEX_PC(vecPos[6], color));
	vecVertexOut.push_back(VERTEX_PC(vecPos[7], color));

	for (size_t i = 0; i < CUBE_INDEX_SIZE; i++)
	{
		vecIndexOut.push_back(g_aCubeIndex[i]);
	}
}

void Zombie::CreateBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex)
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

void Zombie::SetBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex)
{
	VERTEX_PC* pVertex;
	pVb->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, &vecVertex[0], vecVertex.size() * sizeof(VERTEX_PC));
	pVb->Unlock();
	vecVertex.clear();

	WORD* pIndex;
	pIb->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &vecIndex[0], vecIndex.size() * sizeof(WORD));
	pIb->Unlock();
	vecIndex.clear();
}
