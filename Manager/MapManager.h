#pragma once

#define g_MapMGR MapManager::GetInstance()

#define g_pCurrentMap MapManager::GetInstance()->GetCurrentMap()

class DMap;

class MapManager
{
	SINGLETON(MapManager)
private:
	DMap * m_pCurrentMap;
	map<string, DMap*> m_mapList;

public:
	void AddMap(string mapName, DMap* pMap)
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

	DMap* GetCurrentMap() {	return m_pCurrentMap; }
};

