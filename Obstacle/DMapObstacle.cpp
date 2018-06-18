#include "../stdafx.h"
#include "DMapObstacle.h"
#include "../Utility/BDrawingGroup.h"
#include "../Loader/ObjLoader.h"

DMapObstacle::DMapObstacle()
{
}


DMapObstacle::~DMapObstacle()
{
	for (auto p : m_vecDrawingGroup)
		SAFE_RELEASE(p);

	for (auto p : m_vecMtlTex)
		SAFE_RELEASE(p);

	SAFE_RELEASE(m_pMesh);
}

void DMapObstacle::SetObstacle(const char * fileName, D3DXVECTOR3 WorldPos)
{
	m_pos = WorldPos;

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixTranslation(&m_matWorld, m_pos.x, m_pos.y, m_pos.z);

	ObjLoader loader;
	m_pMesh = loader.LoadMesh("Resource/MapObject/", fileName, &m_matWorld, m_vecMtlTex);	// 메쉬 로드 

	// 장애물 태그 추가
	g_DisplayObjMGR->AddObjectWithTag(this, NORMAL_OBSTACLE_TAG);

	cout << m_vecMtlTex.size() << endl;

	// 충돌 검사용 바운싱박스 제작
	m_BoundingBox->initBoundingBox(m_pMesh);
}

void DMapObstacle::RenderMesh()
{
	for (size_t i = 0; i < m_vecMtlTex.size(); i++)
	{
		g_Device->SetMaterial(&m_vecMtlTex[i]->material);
		g_Device->SetTexture(0, m_vecMtlTex[i]->pTexture);
		//g_pDevice->SetFVF(VERTEX_PNT::FVF);
		m_pMesh->DrawSubset(m_vecMtlTex[i]->id);
	}
}

void DMapObstacle::Init()
{
	m_BoundingBox->initBoundingBox(m_pMesh);
}

void DMapObstacle::Update()
{
	// 사실 MapObstacle은 움직이지 않으므로 필요없음
	m_BoundingBox->UpdateBoundingBox(m_matWorld, m_pos);
}

void DMapObstacle::RenderDrawingGroup()
{
	for (auto p : m_vecDrawingGroup)
		SAFE_RENDER(p);
}


void DMapObstacle::Render()
{
	// 빛을 주면 그림이 다 안나와서.. 일단 빛을 뺌
	g_Device->SetRenderState(D3DRS_LIGHTING, true);
	//g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);

	//RenderDrawingGroup();
	RenderMesh();

	m_BoundingBox->RenderBoundingBox();
}
