#include "../stdafx.h"
#include "BDrawingGroup.h"


BDrawingGroup::BDrawingGroup()
{
	m_pVB = NULL;
	m_MtlTex = NULL;
}


BDrawingGroup::~BDrawingGroup()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_MtlTex);
}

void BDrawingGroup::SetVertexBuffer(const vector<VERTEX_PNT>& vec)
{
	g_Device->CreateVertexBuffer(vec.size() * sizeof(VERTEX_PNT), 0, VERTEX_PNT::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	m_numTri = vec.size() / 3;

	VERTEX_PNT* pVertices;
	m_pVB->Lock(0, 0, (LPVOID*)&pVertices, 0);
	memcpy(pVertices, &vec[0], vec.size() * sizeof(VERTEX_PNT));
	m_pVB->Unlock();
}

void BDrawingGroup::Render()
{
	g_Device->SetTexture(0, m_MtlTex->pTexture);
	g_Device->SetMaterial(&m_MtlTex->material);
	g_Device->SetFVF(VERTEX_PNT::FVF);
	g_Device->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PNT));
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_numTri);
}
