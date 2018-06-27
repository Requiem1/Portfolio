#include "../stdafx.h"
#include "DGameUI.h"
#include "DUIObject.h"
#include "UImage.h"
#include "UText.h"
#include <atlstr.h>
#include"../Player/Player.h"


DGameUI::DGameUI()
{
	m_Psprite = NULL;
	m_ProotUI = NULL;
	m_HpBorder = NULL;
	m_HpJuice = NULL;
	m_Hptext = NULL;
}


DGameUI::~DGameUI()
{
	//m_ProotUI->ReleaseAll();
	SAFE_RELEASE(m_Psprite);
	SAFE_RELEASE(m_ProotUI);
}

void DGameUI::Init()
{
	D3DXCreateSprite(g_Device, &m_Psprite);
	m_Player = g_PlayerMGR->GetMainPlayer();

	//HP BORDER
	m_HpBorder = new UImage(m_Psprite);
	m_HpBorder->SetTexture("Resource/UI/HP_Bar.png");
	m_HpBorder->SetPosition(&D3DXVECTOR3(380, 480, 0));
	m_ProotUI = m_HpBorder;

	//HP JUICE
	m_HpJuice = new UImage(m_Psprite);
	m_HpJuice->SetTexture("Resource/UI/HP_Juice.png");
	m_HpJuice->SetPosition(&D3DXVECTOR3(0, 0, 0));
	m_ProotUI->AddChild(m_HpJuice);

	m_Hptext = new UText(g_FontMGR->GetFont(FONT::NORMAL), m_Psprite);
	m_Hptext->m_drawTextFormat = DT_LEFT | DT_TOP | DT_NOCLIP;
	m_Hptext->m_size = D3DXVECTOR2(312, 200);
	m_Hptext->SetPosition(&D3DXVECTOR3(170, -50, 0));
	m_ProotUI->AddChild(m_Hptext);

	UButton * pButton = new UButton(this, m_Psprite);
	m_ProotUI->AddChild(pButton);
	pButton->SetPosition(&D3DXVECTOR3(100, 650, 0));
	pButton->SetTexture("Resource/UI/START_Off.png", "Resource/UI/START_On.png", "Resource/UI/START_Off.png");
}

void DGameUI::Update()
{
	// 줄어드는 hp 값 업데이트
	AddText(m_Player->GetPlayerInfo().HP);
	m_Hptext->m_text = m_str;

	// 줄어드는 hp 값에 따라 hp juice의 크기 업데이트
	if (m_Player->GetPlayerInfo().HP >= 0)
	{
		m_HpJuice->m_size.x = 4 * m_Player->GetPlayerInfo().HP;
	}


	SAFE_UPDATE(m_ProotUI);
}

void DGameUI::Render()
{
	//g_pDevice->SetTexture(0, NULL);
	//m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	//m_pSprite->SetTransform(&m_matWorld);
	//SAFE_RENDER(m_pRootUI);
	//m_pSprite->End();
	g_Device->SetTexture(0, NULL);
	m_Psprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_Psprite->SetTransform(&m_matWorld);
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	SAFE_RENDER(m_ProotUI);
	m_Psprite->End();
}

void DGameUI::OnClick(UButton * pSender)
{
}
void DGameUI::AddText(int str)
{
	if (m_str != "")
	{
		m_str.Empty();
	}
	CString _str = to_string(str).c_str();
	_AddText(_str);
}
void DGameUI::AddText(string str)
{
	CString _str = str.c_str();
	_AddText(_str);
}
void DGameUI::_AddText(CString & str)
{
	m_str.Append(str);
}