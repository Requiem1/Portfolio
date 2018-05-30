#include "../stdafx.h"
#include "UText.h"


UText::UText(LPD3DXFONT m_pFont,LPD3DXSPRITE pSprite) :DUIObject(pSprite),m_Pfont(m_pFont), m_drawTextFormat(DT_CENTER | DT_VCENTER)
{
}

UText::~UText()
{
}

void UText::Render()
{
	RECT rect;
	SetRect(&rect, m_combinedPos.x, m_combinedPos.y, m_combinedPos.x + m_size.x, m_combinedPos.y + m_size.y);

	m_Pfont->DrawText(m_Psprite, m_text, lstrlen(m_text), &rect, m_drawTextFormat, m_color);

	DUIObject::Render();
}
