#include "../stdafx.h"
#include "ObjMap.h"
#include "../Loader/ObjLoader.h"

ObjMap::ObjMap()
{
	m_rayOffsetY = 5.0f;
	m_BisRenderSurface = false;

	// 맵이므로 오브젝트에서 뺀다
	g_DisplayObjMGR->RemoveObject(this);
}


ObjMap::~ObjMap()
{
	for (auto p : m_vecMtlTex)
		SAFE_RELEASE(p);

	SAFE_RELEASE(m_pMeshMap);
}

void ObjMap::Init()
{
	D3DXMATRIXA16 matS, matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);			// 크기 0.1배
	//D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);		// 크기 안바꿈
	matWorld = matS * matWorld;

	// 메쉬 Load, 바닥(surface) 생성
	ObjLoader loader;
	m_pMeshMap = loader.LoadMesh("Resource/Map", m_fileName.c_str(), &matWorld, m_vecMtlTex);
	loader.CreateSurface(m_vecSurface);
}

void ObjMap::Update()
{

}

void ObjMap::Render()
{
	if (GetAsyncKeyState('8') & 0x0001)
		m_BisRenderSurface = !m_BisRenderSurface;

	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);

	if (!m_BisRenderSurface) RenderMesh();		// Mesh로 Render
	else					 RenderSurface();	// Surface 부분을 Render
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


void ObjMap::RenderMesh()
{
	for (size_t i = 0; i < m_vecMtlTex.size(); i++)
	{
		g_Device->SetMaterial(&m_vecMtlTex[i]->material);
		g_Device->SetTexture(0, m_vecMtlTex[i]->pTexture);
		//g_Device->SetFVF(VERTEX_PNT::FVF);
		m_pMeshMap->DrawSubset(m_vecMtlTex[i]->id);
	}
}

void ObjMap::RenderSurface()
{
	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 
		(m_vecSurface.size() / 3) - 1, &m_vecSurface[0], sizeof(D3DXVECTOR3));
}

