#include "../stdafx.h"
#include "GRifle.h"
#include "IGun.h"
#include "Bullet.h"

GRifle::GRifle()
{
}

GRifle::~GRifle()
{
	for (int i = 0; i < m_vecBullet.size(); i++)
		SAFE_RELEASE(m_vecBullet[i]);
	
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

	
	m_fireType = SHOT;
	if (m_isClick)
	{
		for (int i = 0; i < 5; i++)
		{
			IBullet *B;
			B = new IBullet();
			B->Init();
			D3DXVECTOR3 Temp = m_forward;
			SetBullet(&Temp);
			B->SetPos_Direction(m_pos, Temp);
			B->SetClick(m_isClick);
			m_vecBullet.push_back(B);

		}
	}

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, m_scale, m_scale, m_scale);

	m_matWorld = matS * (*m_ParentMat);
	for (int i = 0; i < m_vecBullet.size(); i++)
		m_vecBullet[i]->Update();
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
	for (int i = 0; i < m_vecBullet.size(); i++)
		m_vecBullet[i]->Render();
}
