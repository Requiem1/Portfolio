#pragma once
#include "../Base/BaseObject.h"
#include "../Enemy/Zombie.h"
#define g_ZombieMGR EnemyManager::GetInstance()

class Zombie;

class EnemyManager
{
	SINGLETON(EnemyManager)
private:
	vector<Zombie*> m_vecZombie;
public:
	void init();
	void Update();
	void Render();

	// �������� �޾ƿ� �����͸� ���� �ش� �Լ��� ȣ���Ѵ�.
	// ����� ������ ���� ������ ȣ������ �ʴ´�.
	void AddZombie(int num)
	{
		for (int i = 0; i < num; i++)
		{
			Zombie * Z;
			Z = new Zombie();
			m_vecZombie.push_back(Z);
		}
	
	}
	vector<Zombie*>  GetZombie()
	{
		return m_vecZombie;
	}
	//debug �뵵 ������̸�, �ϳ��� ���� �޾� �Ẹ���� ����
	Zombie* GetMainZombie()
	{
		return m_vecZombie[0];
	}
	//void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	void Destory();
};

