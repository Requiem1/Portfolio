#include "../stdafx.h"
#include "ObjMap.h"
#include "../Loader/ObjLoader.h"

ObjMap::ObjMap()
{
	m_rayOffsetY = 5.0f;
	m_BisRenderSurface = false;

	// ���̹Ƿ� ������Ʈ���� ����
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
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);			// ũ�� 0.1��
	//D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);		// ũ�� �ȹٲ�
	matWorld = matS * matWorld;

	// �޽� Load, �ٴ�(surface) ����
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

	if (!m_BisRenderSurface) RenderMesh();		// Mesh�� Render
	else					 RenderSurface();	// Surface �κ��� Render
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

