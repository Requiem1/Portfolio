#include "../stdafx.h"
#include "GRifle.h"
//#include "Bullet.h"

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
	m_pos.x = m_ParentMat->_41;
	m_pos.y = m_ParentMat->_42;
	m_pos.z = m_ParentMat->_43;

	//m_rotY = m_Camera->GetRotY();

	//D3DXMATRIXA16   m_matRotX, m_matRotY;
	//D3DXMatrixRotationY(&m_matRotY, m_rotY);
	//D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0, 0, 1), &m_matRotY);

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, m_scale, m_scale, m_scale);

	//D3DXMATRIXA16 matR;
	//D3DXMatrixRotationY(&matR, D3DX_PI / 2);

	//m_matWorld = matS * matR * (*m_ParentMat);
	m_matWorld = matS * (*m_ParentMat);
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
