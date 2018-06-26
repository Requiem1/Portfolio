#include "../stdafx.h"
#include "Bullet.h"

IBullet::IBullet()
{
}

IBullet::~IBullet()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

void IBullet::Init()
{
	// 메쉬 혹은 오브젝트 로딩

	m_BClick = false;
	m_moveSpeed = 5.0f;
	m_currMoveSpeedRate = 1.0f;

	vector<D3DXVECTOR3> vecPos;
	for (size_t i = 0; i < CUBE_VERTEX_SIZE; i++)
	{
		vecPos.push_back(g_aCubeVertex[i]);
	}

	// 버텍스 설정
	D3DCOLOR blue = D3DCOLOR_XRGB(0, 102, 255);

	m_vecVertex.push_back(VERTEX_PC(vecPos[0], blue));
	m_vecVertex.push_back(VERTEX_PC(vecPos[1], blue));
	m_vecVertex.push_back(VERTEX_PC(vecPos[2], blue));
	m_vecVertex.push_back(VERTEX_PC(vecPos[3], blue));
	m_vecVertex.push_back(VERTEX_PC(vecPos[4], blue));
	m_vecVertex.push_back(VERTEX_PC(vecPos[5], blue));
	m_vecVertex.push_back(VERTEX_PC(vecPos[6], blue));
	m_vecVertex.push_back(VERTEX_PC(vecPos[7], blue));

	for (int i = 0; i < CUBE_INDEX_SIZE; i++)
	{
		m_vecIndex.push_back(g_aCubeIndex[i]);
	}
	VertexBuffer(m_pVB, m_pIB, m_vecVertex, m_vecIndex);

	m_pVB->GetDesc(&m_VBDesc);
	m_pIB->GetDesc(&m_IBDesc);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixScaling(&m_CSMat, 0.2f, 0.2f, 0.2f);

	g_DisplayObjMGR->AddObjectWithTag(this, BULLET_TAG);

	m_BoundingBox->initBoundingBox(NULL, D3DXVECTOR3(0.01f, 0.01f, 0.01f), m_pos);
}

void IBullet::Update()
{
	if (m_BClick)
	{
		m_pos = m_pos + m_forward * 1.0f* m_moveSpeed*m_currMoveSpeedRate;
		D3DXMatrixTranslation(&m_CTransMat, m_pos.x, m_pos.y, m_pos.z);
		Debug->AddText(m_pos);
		Debug->EndLine();
		m_matWorld = m_CSMat * m_CTransMat;

		m_BoundingBox->UpdateBoundingBox(m_matWorld);
		// 충돌체크!
		IDisplayObject * CollideEnemy = g_DisplayObjMGR->CollideCheckWithTagFunc(this, 1, ENEMY_TAG);
		if (CollideEnemy != NULL)
		{
			// 충돌 감지에서 제외시켜서 연산량을 줄인다
			g_DisplayObjMGR->RemoveObject(CollideEnemy);
			g_DisplayObjMGR->RemoveObjectWithTag(CollideEnemy, ENEMY_TAG);
			g_DisplayObjMGR->RemoveObject(this);
			g_DisplayObjMGR->RemoveObjectWithTag(this, BULLET_TAG);

			// 안보이는 위치의 범위로 옮겨버린다
			CollideEnemy->SetPosition(&D3DXVECTOR3(10000, 10000, 10000));
			this->SetPosition(&D3DXVECTOR3(10000, 10000, 10000));
		}

	}

	
}

void IBullet::Render()
{

	if (m_BClick)
	{
		g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_Device->SetFVF(VERTEX_PC::FVF);
		g_Device->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
		g_Device->SetIndices(m_pIB);
		g_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
			m_VBDesc.Size, 0, m_IBDesc.Size / 3);
		m_BoundingBox->RenderBoundingBox();
	}
}

void IBullet::VertexBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex)
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
