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

	// �������� �޾ƿ� �����͸� ���� �ش� �Լ��� ȣ���Ѵ�.
	// ����� ������ ���� ������ ȣ������ �ʴ´�.
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

