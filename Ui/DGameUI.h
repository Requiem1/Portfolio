#pragma once
#include "../Base/IDisplayObject.h"
#include "UButton.h"

class Player;
class DUIObject;

// DGameUI 클래스는 IDisplayObject를 상속받아 인게임 내에서의 UI를 표현하는 클래스 입니다.
class DGameUI :public IDisplayObject, public UButtonDelegate
{
private:
	LPD3DXSPRITE m_Psprite;
	DUIObject * m_ProotUI;
	LPD3DXFONT m_Pfont;
	vector<Player*>	m_Pplayer;
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
	//void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);
};