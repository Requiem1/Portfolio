#pragma once
#include <stdarg.h>
#define g_DisplayObjMGR DisplayObjManager::GetInstance()

class DisplayObjManager
{
	SINGLETON(DisplayObjManager)

private:
	set<IDisplayObject*> m_AllObstacleList;				// 모든 오브젝트 확인
	map<WORD, vector<IDisplayObject*>> m_ObstacleList;	// 특정 TAG의 오브젝트 확인

	bool m_isBoundingBoxRender;

public:

	void AddObject(IDisplayObject* Obj);
	void AddObjectWithTag(IDisplayObject* Obj, WORD tag);

	void RemoveObject(IDisplayObject* Obj);
	void RemoveObjectWithTag(IDisplayObject* Obj, WORD tag);
	void Destroy();

	// 충돌에 사용 -> 맵 등을 제외한 모든 오브젝트를 STL set으로 반환함
	set<IDisplayObject*> GetAllDisplayObjList() { return m_AllObstacleList; }

	// 태그가 포함된 오브젝트들을 vector로 반환함
	vector<IDisplayObject*> GetDisplayObjListAtTag(WORD tag) { return m_ObstacleList[tag]; }

	// 충돌 검사 -> 충돌한 개체 하나만을 반환함
	IDisplayObject * CollideCheckAllObjectFunc(IDisplayObject * myObj);

	// 충돌 검사 -> 특정 태그들과 충돌한 개체를 하나만 반환
	// CollideCheckWithTagFunc(오브젝트, 태그의 갯수, 태그들)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)
	IDisplayObject * CollideCheckWithTagFunc(IDisplayObject * myObj, int tagNum, ...);

	// 충돌 검사 -> 충돌하고 있는 모든 개체들을 vector에 넣어 반환
	vector<IDisplayObject *> CollideCheckAllObject_ReturnVecFunc(IDisplayObject * myObj);

	// 충돌 검사 -> 충돌하고 있는 특정 태그들의 개체들을 vector에 넣어 반환
	// CollideCheckWithTagFunc(오브젝트, 태그의 갯수, 태그들)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)
	vector<IDisplayObject *> CollideCheckWithTag_ReturnVecFunc(IDisplayObject * myObj, int tag, ...);

	void SetBoundingBoxRender() { m_isBoundingBoxRender = !m_isBoundingBoxRender; }
	bool GetBoundingBoxRender() { return m_isBoundingBoxRender; }
};


enum OBSTACLE_TAG
{
	PLAYER_TAG,
	ENEMY_TAG,
	BULLET_TAG,				// 총알 Object
	NORMAL_OBSTACLE_TAG,	// 부서지지 않는 일반 Obstacle
	EXPLODE_OBSTACLE_TAG,	// 드럼통 등 폭발하는 Obstacle
	PLAYER_WALL_TAG,		// 사용자가 만드는 벽
	MAP_TAG					// 맵
};

