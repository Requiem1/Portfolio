#pragma once
#include "../Base/IDisplayObject.h"
#include "UButton.h"

class Player;
class DUIObject;
class UText;
class UImage;
class Player;

// DGameUI 클래스는 IDisplayObject를 상속받아 인게임 내에서의 UI를 표현하는 클래스 입니다.
class DGameUI :public IDisplayObject, public UButtonDelegate
{
private:
	LPD3DXSPRITE m_Psprite;
	DUIObject * m_ProotUI;
	LPD3DXFONT m_Pfont;
	Player *	m_Player;

	UImage* m_HpBorder;
	UImage* m_HpJuice;
	UText * m_Hptext;

	CString m_str;

	char m_text[128];
public:
	DGameUI();
	~DGameUI();


	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	// IntroUIButton을(를) 통해 상속됨
	virtual void OnClick(UButton * pSender) override;
	void _AddText(CString& str);
	void AddText(string str);
	void AddText(int str);
};