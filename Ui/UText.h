#pragma once
#include "DUIObject.h"

// UI용 텍스트를 표현할 클래스
class UText :public DUIObject
{
private:
	LPD3DXFONT m_Pfont;
	LPCTSTR m_text;
	DWORD  m_drawTextFormat;

public:
	UText(LPD3DXFONT m_pFont, LPD3DXSPRITE pSprite);
	virtual	~UText();

	virtual void Render() override;
};
