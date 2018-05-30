#pragma once
#include <stdarg.h>
#define g_DisplayObjMGR DisplayObjManager::GetInstance()

class DisplayObjManager
{
	SINGLETON(DisplayObjManager)

private:
	set<IDisplayObject*> m_AllObstacleList;				// 모든 오브젝트 확인
	map<WORD, vector<IDisplayObject*>> m_ObstacleList;	// 특정 TAG의 오브젝트 확인

	bool isBoundingBoxRender;

public:

	void AddObject(IDisplayObject* Obj);
	void AddObjectWithTag(IDisplayObject* Obj, WORD tag);

	void RemoveObject(IDisplayObject* Obj);
	void RemoveObjectWithTag(IDisplayObject* Obj, WORD tag);
	void Destroy();

	set<IDisplayObject*> GetAllDisplayObjList() { return m_AllObstacleList; }
	vector<IDisplayObject*> GetDisplayObjListAtTag(WORD tag) { return m_ObstacleList[tag]; }

	// 충돌 검사 함수!
	IDisplayObject * CollideCheckAllObjectFunc(IDisplayObject * myObj);
	IDisplayObject * CollideCheckWithTagFunc(IDisplayObject * myObj, int tagNum, ...);	
	// ㄴWithTag 함수 - 여러개의 태그를 가변적으로 넣을 수 있다
	// CollideCheckWithTagFunc(오브젝트, 태그의 갯수, 태그들)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)

	// 충돌 검사 -> 충돌하고 있는 모든 개체들을 vector에 넣어
	vector<IDisplayObject *> CollideCheckAllObject_ReturnVecFunc(IDisplayObject * myObj);
	vector<IDisplayObject *> CollideCheckWithTag_ReturnVecFunc(IDisplayObject * myObj, int tag, ...);

	void SetBoundingBoxRender() { isBoundingBoxRender = !isBoundingBoxRender; }
	bool GetBoundingBoxRender() { return isBoundingBoxRender; }
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

