#pragma once
#include "DUIObject.h"

// UI�� �ؽ�Ʈ�� ǥ���� Ŭ����
class UText :public DUIObject
{
public:
	UText(LPD3DXFONT m_pFont, LPD3DXSPRITE pSprite);
	DWORD  m_drawTextFormat;
	LPCTSTR m_text;
	LPD3DXFONT m_Pfont;

	virtual	~UText();

	virtual void Render() override;
};
