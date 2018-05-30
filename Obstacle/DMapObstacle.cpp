#include "../stdafx.h"
#include "MapObstacle.h"
#include "DrawingGroup.h"
#include "../Loader/ObjLoader.h"

MapObstacle::MapObstacle()
{
}


MapObstacle::~MapObstacle()
{
	for (auto p : m_vecDrawingGroup)
		SAFE_RELEASE(p);

	for (auto p : m_vecMtlTex)
		SAFE_RELEASE(p);

	SAFE_RELEASE(m_pMesh);
}

void MapObstacle::SetObstacle(const char * fileName, D3DXVECTOR3 WorldPos)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._41 = WorldPos.x;
	m_matWorld._42 = WorldPos.y;
	m_matWorld._43 = WorldPos.z;

	ObjLoader loader;
	m_pMesh = loader.LoadMesh("Resource/MapObject", fileName, &m_matWorld, m_vecMtlTex);	// 메쉬 로드 

	// 장애물 태그 추가
	g_DisplayObjMgr->AddObjectWithTag(this, NORMAL_OBSTACLE_TAG);

	cout << m_vecMtlTex.size() << endl;

	// 충돌 검사용 바운싱박스 제작
	BoundingBox->initBoundingBox(m_pMesh);
}

void MapObstacle::RenderMesh()
{
	for (size_t i = 0; i < m_vecMtlTex.size(); i++)
	{
		g_pDevice->SetMaterial(&m_vecMtlTex[i]->material);
		g_pDevice->SetTexture(0, m_vecMtlTex[i]->pTexture);
		//g_pDevice->SetFVF(VERTEX_PNT::FVF);
		m_pMesh->DrawSubset(m_vecMtlTex[i]->id);
	}
}

void MapObstacle::Init()
{
	BoundingBox->initBoundingBox(m_pMesh);
}

void MapObstacle::Update()
{
	BoundingBox->UpdateBoundingBox(m_matWorld, m_pos);
}

void MapObstacle::RenderDrawingGroup()
{
	for (auto p : m_vecDrawingGroup)
		SAFE_RENDER(p);
}


void MapObstacle::Render()
{
	// 빛을 주면 그림이 다 안나와서.. 일단 빛을 뺌
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	//RenderDrawingGroup();
	RenderMesh();

	BoundingBox->RenderBoundingBox();
}
