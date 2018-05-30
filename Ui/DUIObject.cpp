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
	// 부모 오브젝트가 있으면 그 위치에 맞게 설정후
	//상속받은 애들 업데이트
	for (auto p : m_vecPChild)
	{
		p->Update();
	}
}

void DUIObject::Render()
{
	// 테두리가 있다면 그리고
	if (m_bDrawBorder)DrawBorder();
	//상속받은 애들 render
	for (auto p : m_vecPChild)
	{
		p->Render();
	}
}

void DUIObject::DrawBorder()
{
	// UI를 구성하는 전체의 테두리를 그리는 함수
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
	//내 포지션을 기본으로 저장한 후, 만약 부모가 있다면 부모의 위치를 더해 준다.
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

	//스프라이트 이미지의 행렬을 받아와
	//카메라 좌표에 맞게 계산해 
	// 최종적인 좌표를 만든다.

	int left = mat._41 + m_combinedPos.x * mat._11;
	int top = mat._42 + m_combinedPos.y* mat._22;
	int right = left + m_size.x* mat._11;
	int bottom = top + m_size.y* mat._22;


	SetRect(rect, left, top, right, bottom);
}
