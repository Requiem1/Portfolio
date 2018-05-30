#include "../stdafx.h"
#include "DUIObject.h"

DUIObject::DUIObject(LPD3DXSPRITE Psprite)
	:m_bDrawBorder(true),m_Psprite(Psprite),m_color(D3DXCOLOR(1,1,1,1))
{
}

DUIObject::~DUIObject()
{

}

void DUIObject::Init()
{
}

void DUIObject::Update()
{
	UpdateCombinedPosition();
	// �θ� ������Ʈ�� ������ �� ��ġ�� �°� ������
	//��ӹ��� �ֵ� ������Ʈ
	for (auto p : m_vecPChild)
	{
		p->Update();
	}
}

void DUIObject::Render()
{
	// �׵θ��� �ִٸ� �׸���
	if (m_bDrawBorder)DrawBorder();
	//��ӹ��� �ֵ� render
	for (auto p : m_vecPChild)
	{
		p->Render();
	}
}

void DUIObject::DrawBorder()
{
	// UI�� �����ϴ� ��ü�� �׵θ��� �׸��� �Լ�
	RECT r;
	GetFinalRect(&r);

	vector<VERTEX_RHWC> vecVertex;
	vecVertex.reserve(5);
	D3DCOLOR c = WHITE;

	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(r.left, r.top, 0, 1), c));
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(r.right, r.top, 0, 1), c));
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(r.right, r.bottom, 0, 1), c));
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(r.left, r.bottom, 0, 1), c));
	vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(r.left, r.top, 0, 1), c));

	g_Device->SetFVF(VERTEX_RHWC::FVF);
	g_Device->DrawPrimitiveUP(D3DPT_LINESTRIP, vecVertex.size() - 1, &vecVertex[0], sizeof(VERTEX_RHWC));
}

void DUIObject::UpdateCombinedPosition()
{
	//�� �������� �⺻���� ������ ��, ���� �θ� �ִٸ� �θ��� ��ġ�� ���� �ش�.
	m_combinedPos = m_pos;
	if (m_pParent)
	{
		m_combinedPos += static_cast<DUIObject*>(m_pParent)->GetCombinedPosition();

	}
}

void DUIObject::GetFinalRect(RECT * rect)
{
	D3DXMATRIXA16 mat;
	m_Psprite->GetTransform(&mat);

	//��������Ʈ �̹����� ����� �޾ƿ�
	//ī�޶� ��ǥ�� �°� ����� 
	// �������� ��ǥ�� �����.

	int left = mat._41 + m_combinedPos.x * mat._11;
	int top = mat._42 + m_combinedPos.y* mat._22;
	int right = left + m_size.x* mat._11;
	int bottom = top + m_size.y* mat._22;


	SetRect(rect, left, top, right, bottom);
}
