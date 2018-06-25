#include "../stdafx.h"
#include "EnemyManager.h"
#include "../Enemy/Zombie.h"


EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::init()
{
	for (size_t i = 0; i < m_vecZombie.size(); i++)
	{
		m_vecZombie[i]->SetXfile("Resource/Enemy/", "Zombie1.x", 0.125f);
		m_vecZombie[i]->Init();
	}
}

void EnemyManager::Update()
{
	for (size_t i = 0; i < m_vecZombie.size(); i++)
		m_vecZombie[i]->Update();
}

void EnemyManager::Render()
{
	for (size_t i = 0; i < m_vecZombie.size(); i++)
		m_vecZombie[i]->Render();
}

void EnemyManager::Destory()
{
	for (int i = 0; i < m_vecZombie.size(); i++)
		SAFE_RELEASE(m_vecZombie[i]);
}
