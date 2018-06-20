#include "../stdafx.h"
#include "ObjMap.h"
#include "../Utility/BDrawingGroup.h"
#include "../Loader/ObjLoader.h"

ObjMap::ObjMap()
{
	m_rayOffsetY = 4.0f;
}


ObjMap::~ObjMap()
{
	for (auto p : m_vecDrawingGroup)
		SAFE_RELEASE(p);

	for (auto p : m_vecMtlTex)
		SAFE_RELEASE(p);

	SAFE_RELEASE(m_pMeshMap);
}

void ObjMap::Init()
{
	D3DXMATRIXA16 matRX, matRY, matS, matWorld;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixRotationX(&matRX, -D3DX_PI / 2.0f);
	//D3DXMatrixRotationY(&matRY, D3DX_PI / 2.0f);
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);	// 크기 0.1배
	//D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);		// 크기 안바꿈

	matWorld = matS * matWorld;// matRX * matRY;

	// 메쉬 Load, 바닥(surface) 생성
	ObjLoader loader;
	m_pMeshMap = loader.LoadMesh("Resource/Map", m_fileName.c_str(), &matWorld, m_vecMtlTex);
	loader.CreateSurface(m_vecSurface);
	//loader.Load("resources/obj", "Map.obj", &matWorld, m_vecDrawingGroup);			// PNT 로드
	//loader.LoadSurface("Resource/Map/map_surface.obj", &matWorld, m_vecVertex);		// 높이 확인용 Surface 로드
}

void ObjMap::Update()
{

}

void ObjMap::Render()
{
	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);

	//RenderDrawingGroup();	// PNT로 Render
	RenderMesh();			// Mesh로 Render
	//RenderSurface();		// Surface 부분을 Render
}


bool ObjMap::GetHeight(OUT float & height, const D3DXVECTOR3 & pos)
{
	// position 0 1 2 삼각형을 만들고
	// rayPos 위치의 rayDir방향으로 ray를 쏨
	// 높이체크이므로 하단으로 ray를 쏴야하므로 rayDir은 (0, -1, 0)

	D3DXVECTOR3 rayPos(pos.x, pos.y + m_rayOffsetY, pos.z);
	D3DXVECTOR3 rayDir(0, -1, 0);
	float distance;

	for (size_t i = 0; i < m_vecSurface.size(); i += 3)
	{
		// distance가 계산되서 값이 들어가진다!
		if (D3DXIntersectTri(&m_vecSurface[i], &m_vecSurface[i + 1], &m_vecSurface[i + 2], &rayPos, &rayDir, 0, 0, &distance) == true)
		{
			// Pos은 위쪽에 있다
			// 교차하면 움직이고, 교차하지않으면 움직이지 않음
			height = rayPos.y - distance;
			return true;
		}
	}

	return false;
}

void ObjMap::RenderDrawingGroup()
{
	for (auto p : m_vecDrawingGroup)
		SAFE_RENDER(p);
}

void ObjMap::RenderMesh()
{
	for (size_t i = 0; i < m_vecMtlTex.size(); i++)
	{
		g_Device->SetMaterial(&m_vecMtlTex[i]->material);
		g_Device->SetTexture(0, m_vecMtlTex[i]->pTexture);
		//g_pDevice->SetFVF(VERTEX_PNT::FVF);
		m_pMeshMap->DrawSubset(m_vecMtlTex[i]->id);
	}
}

void ObjMap::RenderSurface()
{
	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 
		(m_vecSurface.size() / 3) - 1, &m_vecSurface[0], sizeof(D3DXVECTOR3));
}

