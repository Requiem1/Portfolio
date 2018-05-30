#include "../stdafx.h"
#include "DGameUI.h"
#include "DUIObject.h"
#include "UImage.h"
#include "UText.h"
#include <atlstr.h>



DGameUI::DGameUI()
{
	//m_pSprite = NULL;
	//m_pRootUI = NULL;
}


DGameUI::~DGameUI()
{
	//m_pRootUI->ReleaseAll();
	//SAFE_RELEASE(m_pSprite);
	//SAFE_RELEASE(m_pRootUI);
}

void DGameUI::Init()
{
	//D3DXCreateSprite(g_pDevice, &m_pSprite);
	//
	//m_pPlayer = g_pPlayerMGR->GetPlayer();

	//UIImage* pImage = new UIImage(m_pSprite);
	////pImage->SetTexture("UI/resources/panel-info.png.png");
	//pImage->SetPosition(&D3DXVECTOR3(0, 0, 0));
	//m_pRootUI = pImage;

	//UIText *pText = new UIText(g_FontMgr->GetFont(FONT::NORMAL),m_pSprite);
	////string a;
	////_itoa_s(m_pPlayer[0]->GetPlayerInfo().HP, m_text, 10);
	//wchar_t LPW[128];
	////CharToWChar(m_text, LPW);
	////a = m_text;
	////LPCTSTR x = new TCHAR[4];
	////strcpy(x, a.c_str());
	////tstring z = TEXT("Hello");
	////a = m_text;
	////z = a;
	////LPTSTR x = new TCHAR[z.size() + 1];
	////	_tcscpy(x, z.c_str());
	////m_pRootUI->AddChild(pText);

	//pText->m_drawTextFormat = DT_LEFT | DT_TOP | DT_NOCLIP;
	////LPCTSTR LP = CA2W(m_text);
	//pText->m_text = LPW;
	//pText->m_size = D3DXVECTOR2(312, 200);
	//pText->SetPosition(&D3DXVECTOR3(100, 100, 0));

	//D3DXMATRIXA16 matS;
	//D3DXMatrixScaling(&matS, 0.7f, 0.7, 1);

	//D3DXMATRIXA16 matT;
	//D3DXMatrixTranslation(&matT, -100, 240, 0);
	//m_matWorld = matS * matT;
}

void DGameUI::Update()
{
	//SAFE_UPDATE(m_pRootUI);
}

void DGameUI::Render()
{
	//g_pDevice->SetTexture(0, NULL);
	//m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	//m_pSprite->SetTransform(&m_matWorld);
	//SAFE_RENDER(m_pRootUI);
	//m_pSprite->End();
}

void DGameUI::OnClick(UButton * pSender)
{
}
