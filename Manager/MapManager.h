#pragma once

#define g_MapMGR MapManager::GetInstance()

#define g_pCurrentMap MapManager::GetInstance()->GetCurrentMap()

class IMap;

class MapManager
{
	SINGLETON(MapManager)
private:
	IMap * m_pCurrentMap;
	map<string, IMap*> m_mapList;

	vector<vector<D3DXVECTOR3>> m_vecEnemyspawnList;

public:
	void AddMap(string mapName, IMap* pMap)
	{
		// ���� �̹� �ִٸ� return
		if (m_mapList.count(mapName) == 1)
			return;

		m_mapList[mapName] = pMap;
	}

	void SetCurrentMap(string mapName)
	{
		m_pCurrentMap = m_mapList[mapName];
	}

	// ���ʹ̽����� üũ����Ʈ���� �������� �����ϹǷ�
	// üũ����Ʈ�� ũ��� ���� resize�� ���ش�
	void SetEnemyspawnList(int CheckpointNum) 
	{
		m_vecEnemyspawnList.resize(CheckpointNum);
	}

	// �� üũ����Ʈ ��ġ�� ���ʹ̽����� push�Ѵ�
	void PushEnemyspawn(int nowCheckpoint, D3DXVECTOR3 spawnPos)
	{
		m_vecEnemyspawnList[nowCheckpoint].push_back(spawnPos);
	}

	// üũ����Ʈ�� �´� ���ʹ̽��� pos���� �����´�
	vector<D3DXVECTOR3> GetEnemyspawnOnCheckpoint(int nowCheckpoint)
	{
		return m_vecEnemyspawnList[nowCheckpoint];
	}

	IMap* GetCurrentMap() {	return m_pCurrentMap; }
};

