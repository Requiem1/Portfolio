#include "../stdafx.h"
#include "Item.h"
#include "../Loader/ObjLoader.h"

DITEM::DITEM()
{

}

DITEM::~DITEM()
{
	SAFE_RELEASE(m_Mesh);
}

void DITEM::LoadMesh(const char * Path, const char * fileName, float ScaleXYZ)
{
	D3DXMATRIXA16  m_CSMat;
	D3DXMatrixScaling(&m_CSMat, ScaleXYZ, ScaleXYZ, ScaleXYZ);
	m_scale = ScaleXYZ;
	
	D3DXMATRIXA16 TempWorld;
	//D3DXMatrixRotationY(&matBaseR, D3DX_PI);
	D3DXMatrixIdentity(&m_matWorld);

	TempWorld = matBaseR * m_CSMat;

	ObjLoader loader;
	m_Mesh = loader.LoadMesh(Path, fileName, &m_matWorld, m_vecMtlTex);
	
	if (m_Mesh == NULL)
		cout << "아이템 메시정보 로드 실패" << endl;
}

