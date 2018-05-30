#include "../stdafx.h"
#include "UImage.h"


UImage::UImage(LPD3DXSPRITE Psprite, int uiTag) :DUIObject(Psprite), m_Ptex(NULL)
{
}

UImage::~UImage()
{
	SAFE_RELEASE(m_Ptex);
}

void UImage::Render()
{
	if (m_Ptex)
	{
		RECT rect;
		//텍스쳐의 사이즈
		SetRect(&rect, 0, 0, m_size.x, m_size.y);

		m_Psprite->Draw(m_Ptex, &rect, &m_pivot, &m_combinedPos, m_color);
	}
	// 루트가 호출해 주면 차일드에 렌더로 간다.
	DUIObject::Render();
}

void UImage::SetTexture(string fullPath)
{
	D3DXIMAGE_INFO info;
	m_Ptex = g_TextureMGR->GetTexture(fullPath);
	D3DXGetImageInfoFromFileA(fullPath.c_str(), &info);
	m_size.x = info.Width;
	m_size.y = info.Height;
}
