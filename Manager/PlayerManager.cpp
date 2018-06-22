#include "../stdafx.h"
#include "PlayerManager.h"
#include "../Player/Player.h"

PlayerManager::PlayerManager()
{
}


PlayerManager::~PlayerManager()
{
}

void PlayerManager::init()
{

}

void PlayerManager::Update()
{
	for (size_t i = 0; i < m_vecPlayer.size(); i++)
		m_vecPlayer[i]->Update();
}

void PlayerManager::Render()
{
	for (size_t i = 0; i < m_vecPlayer.size(); i++)
		m_vecPlayer[i]->Render();
}

void PlayerManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	for (size_t i = 0; i < m_vecPlayer.size(); i++)
		m_vecPlayer[i]->WndProc(hWnd, message, wParam, lParam);
}

void PlayerManager::Destory()
{
	//for (int i = 0; i < m_vecPlayer.size(); i++)
	//	SAFE_RELEASE(m_vecPlayer[i]);
}
