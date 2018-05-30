#pragma once
#include "../Base/IDisplayObject.h"
#include "UButton.h"

class DUIObject;

// DGameUI Ŭ������ IDisplayObject�� ��ӹ޾� �ΰ��� �������� UI�� ǥ���ϴ� Ŭ���� �Դϴ�.
class DIntroUI :public IDisplayObject , public UButtonDelegate
{
private:
	LPD3DXSPRITE m_Psprite;
	DUIObject * m_ProotUI;
public:
	DIntroUI();
	~DIntroUI();


	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	// IntroUIButton��(��) ���� ��ӵ�
	virtual void OnClick(UButton * pSender) override;
};

