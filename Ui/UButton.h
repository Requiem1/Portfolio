#pragma once
#include "DUIObject.h"

class UButton;

class UButtonDelegate
{
public:
	virtual void OnClick(UButton* Psender) = 0;
};

class UButton : public DUIObject
{
private:
	enum ButtonState
	{
		MOUSE_ON, MOUSE_OFF,MOUSE_CLICKED,COUNT
	};
	ButtonState m_buttonState;
public:
	UButtonDelegate *m_Pdelegate;
	//COUNT ������ŭ ���¿� ���� �ٸ��� ���̵���
	LPDIRECT3DTEXTURE9 m_aTexture[COUNT-1];

	UButton(UButtonDelegate* Pdelegate, LPD3DXSPRITE Psprite);
	virtual	~UButton();

	virtual void Update() override;
	virtual void Render() override;

	void SetTexture(string normal, string mouseOver, string selected);
	void SetText(LPD3DXFONT font, LPCTSTR text);
};

