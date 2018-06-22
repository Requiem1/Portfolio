#include "../stdafx.h"
#include "GRifle.h"
#include "Bullet.h"
GRifle::GRifle()
{
}

GRifle::~GRifle()
{
}

void GRifle::Init()
{
	CurrentSize = 0;
	FullSize = 30;
}

void GRifle::Update()
{


	D3DXVECTOR3 m_temp;
	m_temp.x = m_ParentMat->_41;
	m_temp.y = m_ParentMat->_42;
	m_temp.z = m_ParentMat->_43;
	
	m_pos = m_temp;

	D3DXVec3Normalize(&m_forward, &m_temp);
	m_pos += m_forward;

	D3DXMATRIXA16 m_Trans;
	D3DXMatrixTranslation(&m_Trans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMATRIXA16  m_CSMat;
	D3DXMatrixScaling(&m_CSMat, m_scale, m_scale, m_scale);

	m_matWorld = m_CSMat* m_Trans;

	
	
}

void GRifle::Render()
{
	for (size_t i = 0; i < m_vecMtlTex.size(); i++)
	{
		g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_Device->SetMaterial(&m_vecMtlTex[i]->material);
		g_Device->SetTexture(0, m_vecMtlTex[i]->pTexture);
		m_Mesh->DrawSubset(m_vecMtlTex[i]->id);
	}
}
