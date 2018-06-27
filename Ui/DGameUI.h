#pragma once
#include "../Base/IDisplayObject.h"
#include "UButton.h"

class Player;
class DUIObject;
class UText;
class UImage;
class Player;

// DGameUI Ŭ������ IDisplayObject�� ��ӹ޾� �ΰ��� �������� UI�� ǥ���ϴ� Ŭ���� �Դϴ�.
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


	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	// IntroUIButton��(��) ���� ��ӵ�
	virtual void OnClick(UButton * pSender) override;
	void _AddText(CString& str);
	void AddText(string str);
	void AddText(int str);
};