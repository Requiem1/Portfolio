#pragma once
#include "../Base/BaseObject.h"
#define g_PlayerMGR PlayerManager::GetInstance()

class Player;

class PlayerManager
{
	SINGLETON(PlayerManager)
private:
	vector<Player*> m_vecPlayer;
public:
	void init();
	void Update();
	void Render();

	// 서버에서 받아온 데이터를 통해 해당 함수를 호출한다.
	// 현재는 서버가 없기 때문에 호출하지 않는다.
	void AddPlayer(Player* Player)
	{
		m_vecPlayer.push_back(Player);
	}
	vector<Player*>  GetPlayer()
	{
		return m_vecPlayer;
	}
	Player* GetMainPlayer()
	{
		return m_vecPlayer[0];
	}
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	void Destory();
};

