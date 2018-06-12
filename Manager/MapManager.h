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
		// 맵이 이미 있다면 return
		if (m_mapList.count(mapName) == 1)
			return;

		m_mapList[mapName] = pMap;
	}

	void SetCurrentMap(string mapName)
	{
		m_pCurrentMap = m_mapList[mapName];
	}

	// 에너미스폰은 체크포인트마다 여러개가 존재하므로
	// 체크포인트의 크기로 먼저 resize를 해준다
	void SetEnemyspawnList(int CheckpointNum) 
	{
		m_vecEnemyspawnList.resize(CheckpointNum);
	}

	// 각 체크포인트 위치에 에너미스폰을 push한다
	void PushEnemyspawn(int nowCheckpoint, D3DXVECTOR3 spawnPos)
	{
		m_vecEnemyspawnList[nowCheckpoint].push_back(spawnPos);
	}

	// 체크포인트에 맞는 에너미스폰 pos들을 가져온다
	vector<D3DXVECTOR3> GetEnemyspawnOnCheckpoint(int nowCheckpoint)
	{
		return m_vecEnemyspawnList[nowCheckpoint];
	}

	IMap* GetCurrentMap() {	return m_pCurrentMap; }
};

