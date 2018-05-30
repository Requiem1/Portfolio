#pragma once
#include "../Base/IDisplayObject.h"
#include "UButton.h"

class Player;
class DUIObject;

// DGameUI Ŭ������ IDisplayObject�� ��ӹ޾� �ΰ��� �������� UI�� ǥ���ϴ� Ŭ���� �Դϴ�.
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


	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	// IntroUIButton��(��) ���� ��ӵ�
	virtual void OnClick(UButton * pSender) override;
	//void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);
};