#include "../stdafx.h"
#include "DIntroUI.h"
#include "DUIObject.h"
#include "UImage.h"

//enum
//{
//	UITAG_TEXTVIEW,
//	UITAG_BUTTON1,
//	UITAG_BUTTON2
//};

DIntroUI::DIntroUI()
{
	m_Psprite = NULL;
	m_ProotUI = NULL;
}


DIntroUI::~DIntroUI()
{
	m_ProotUI->ReleaseAll();
	SAFE_RELEASE(m_Psprite);
	SAFE_RELEASE(m_ProotUI);
}

void DIntroUI::Init()
{
	D3DXCreateSprite(g_Device, &m_Psprite);

	UImage* pImage = new UImage(m_Psprite);
	//pImage->SetTexture("UI/resources/panel-info.png.png");
	pImage->SetPosition(&D3DXVECTOR3(0, 0, 0));
	m_ProotUI = pImage;

	UButton * pButton = new UButton(this, m_Psprite);
	m_ProotUI->AddChild(pButton);
	pButton->SetPosition(&D3DXVECTOR3(135, 330, 0));
	pButton->SetTexture("../Resource/UI/START_Off.png", "../Resource/UI/START_On.png", "../Resource/UI/START_Off.png");

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.7f, 0.7, 1);

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, -100, 240, 0);
	m_matWorld = matS * matT;
}

void DIntroUI::Update()
{
	SAFE_UPDATE(m_ProotUI);
}

void DIntroUI::Render()
{
	g_Device->SetTexture(0, NULL);
	m_Psprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_Psprite->SetTransform(&m_matWorld);
	SAFE_RENDER(m_ProotUI);
	m_Psprite->End();
}

void DIntroUI::OnClick(UButton * pSender)
{
}
