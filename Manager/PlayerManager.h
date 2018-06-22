#pragma once
#include "../Base/BaseObject.h"
#define g_PlayerMGR PlayerManager::GetInstance()

class Player;

class PlayerManager
{
	SINGLETON(PlayerManager)
private:
	vector<Player*>		m_vecPlayer;
	vector<D3DXVECTOR3> m_vecStartPos;
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

	void AddStartPos(D3DXVECTOR3 s) { m_vecStartPos.push_back(s); }
	D3DXVECTOR3 GetPlayerStartPos(int n) { return m_vecStartPos[n]; }

	void Destory();
};

enum PLAYERENUM
{
	INDEX_PLAYER_1,
	INDEX_PLAYER_2,
	INDEX_PLAYER_3,
	INDEX_PLAYER_4
};