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

	// 서버에서 받아온 데이터를 통해 해당 함수를 호출한다.
	// 현재는 서버가 없기 때문에 호출하지 않는다.
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
	//debug 용도 지울것이며, 하나의 좀비 받아 써보고자 만듬
	Zombie* GetMainZombie()
	{
		return m_vecZombie[0];
	}
	//void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	void Destory();
};

