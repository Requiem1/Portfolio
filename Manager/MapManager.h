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

	IMap* GetCurrentMap() {	return m_pCurrentMap; }
};

