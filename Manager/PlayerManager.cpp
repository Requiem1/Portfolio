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
	Player * P;
	P = new Player();
	P->Init();
	m_vecPlayer.push_back(P);
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

void PlayerManager::Destory()
{
	for (int i = 0; i < m_vecPlayer.size(); i++)
		SAFE_RELEASE(m_vecPlayer[i]);
}
