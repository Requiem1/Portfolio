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
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);	// ũ�� 0.1��
	//D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);		// ũ�� �ȹٲ�

	matWorld = matS * matWorld;// matRX * matRY;

	// �޽� Load, �ٴ�(surface) ����
	ObjLoader loader;
	m_pMeshMap = loader.LoadMesh("Resource/Map", m_fileName.c_str(), &matWorld, m_vecMtlTex);
	loader.CreateSurface(m_vecSurface);
	//loader.Load("resources/obj", "Map.obj", &matWorld, m_vecDrawingGroup);			// PNT �ε�
	//loader.LoadSurface("Resource/Map/map_surface.obj", &matWorld, m_vecVertex);		// ���� Ȯ�ο� Surface �ε�
}

void ObjMap::Update()
{

}

void ObjMap::Render()
{
	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);

	//RenderDrawingGroup();	// PNT�� Render
	RenderMesh();			// Mesh�� Render
	//RenderSurface();		// Surface �κ��� Render
}


bool ObjMap::GetHeight(OUT float & height, const D3DXVECTOR3 & pos)
{
	// position 0 1 2 �ﰢ���� �����
	// rayPos ��ġ�� rayDir�������� ray�� ��
	// ����üũ�̹Ƿ� �ϴ����� ray�� �����ϹǷ� rayDir�� (0, -1, 0)

	D3DXVECTOR3 rayPos(pos.x, pos.y + m_rayOffsetY, pos.z);
	D3DXVECTOR3 rayDir(0, -1, 0);
	float distance;

	for (size_t i = 0; i < m_vecSurface.size(); i += 3)
	{
		// distance�� ���Ǽ� ���� ������!
		if (D3DXIntersectTri(&m_vecSurface[i], &m_vecSurface[i + 1], &m_vecSurface[i + 2], &rayPos, &rayDir, 0, 0, &distance) == true)
		{
			// Pos�� ���ʿ� �ִ�
			// �����ϸ� �����̰�, �������������� �������� ����
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

