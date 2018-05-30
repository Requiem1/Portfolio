#include "../stdafx.h"
#include "UButton.h"
#include "../Map/MHeightMap.h"


UButton::UButton(UButtonDelegate * Pdelegate, LPD3DXSPRITE Psprite)
	:DUIObject(Psprite), m_Pdelegate(Pdelegate), m_buttonState(MOUSE_OFF)
{
}

UButton::~UButton()
{
}

void UButton::Update()
{
	DUIObject::Update();

	//��Ʈ�� ��ư�� ��ġ�� ������ ���� �־���
	RECT rect;
	GetFinalRect(&rect);

	POINT mousePoint;
	//������ ������ �ƴ� ��ũ�� ������
	GetCursorPos(&mousePoint);
	ScreenToClient(g_hWnd, &mousePoint);

	//���콺�����Ͱ� ��ư ���� �ȿ� �ִٸ�
	if (PtInRect(&rect, mousePoint))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{    //��ư���� ���콺�� �÷��� ���¶��
			if (m_buttonState == MOUSE_ON)
			{
				m_buttonState = MOUSE_CLICKED;
			}
		}
		else
		{
			if (m_buttonState == MOUSE_CLICKED)
			{
				if (m_Pdelegate)
					m_Pdelegate->OnClick(this);
				g_SceneMGR->SetCurrentScene(SCENE_HEIGHTMAP);
			}
			m_buttonState = MOUSE_ON;
		}
	}
	else // ���콺 �����Ͱ� ��ư ���� �ۿ� ���� ��
	{//��ư�� �������¿��� ���� ������ ������ ���  �����ϱ� ���Ͽ�
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{

		}
		else
		{
			m_buttonState = MOUSE_OFF;
		}
	}

}

void UButton::Render()
{
	if (m_aTexture[m_buttonState])
	{
		RECT rect;
		SetRect(&rect, 0, 0, m_size.x, m_size.y);
		m_Psprite->Draw(m_aTexture[m_buttonState], &rect, &m_pivot, &m_combinedPos, m_color);
	}
	DUIObject::Render();
}

void UButton::SetTexture(string normal, string mouseOver, string selected)
{
	D3DXIMAGE_INFO info;
	m_aTexture[MOUSE_OFF] = g_TextureMGR->GetTexture(normal);
	m_aTexture[MOUSE_ON] = g_TextureMGR->GetTexture(mouseOver);
	m_aTexture[MOUSE_CLICKED] = g_TextureMGR->GetTexture(selected);

	D3DXGetImageInfoFromFileA(normal.c_str(), &info);
	m_size.x = info.Width;
	m_size.y = info.Height;
}

void UButton::SetText(LPD3DXFONT font, LPCTSTR text)
{
}
