#pragma once
#include <stdarg.h>
#define g_DisplayObjMGR DisplayObjManager::GetInstance()

class DisplayObjManager
{
	SINGLETON(DisplayObjManager)

private:
	set<IDisplayObject*> m_AllObstacleList;				// 모든 오브젝트 확인
	map<WORD, vector<IDisplayObject*>> m_ObstacleList;	// 특정 TAG의 오브젝트 확인

	map<string, IDisplayObject*> m_LoadObjList;			// 로드된 .Obj파일들을 저장함

	bool m_BisBoundingBoxRender;

public:
	// 매니저에 오브젝트 포인터 추가 -> IDisplayObject에서 자동 계산함
	void AddObject(IDisplayObject* Obj);

	// 각 객체를 태그를 달아서 추가!!! 태그가 필요한 객체는 모두 넣어야됨
	void AddObjectWithTag(IDisplayObject* Obj, WORD tag);

	void RemoveObject(IDisplayObject* Obj);
	void RemoveObjectWithTag(IDisplayObject* Obj, WORD tag);
	void Destroy();


	// .Obj 파일이 이미 로드되었는지 확인
	// 로드된 파일이라면 한번 더 로드할 필요없이 복사만 해서 넘기면 되므로...
	// 깊은 복사를 해야됨을 기억하자
	IDisplayObject* isObjectLoaded(string str);


	// ------------------------------------------------------------------
	//                  아래는 [충돌] 관련 함수들입니다
	// ------------------------------------------------------------------
	
	// 충돌에 사용 -> 맵 등을 제외한 모든 오브젝트를 STL set으로 반환함
	set<IDisplayObject*> GetAllDisplayObjList() { return m_AllObstacleList; }

	// 태그가 포함된 오브젝트들을 vector로 반환함
	vector<IDisplayObject*> GetDisplayObjListAtTag(WORD tag) { return m_ObstacleList[tag]; }

	// 충돌 검사 -> 충돌한 개체 하나만을 반환함
	IDisplayObject * CollideCheckAllObjectFunc(IDisplayObject * myObj);


	// 충돌 검사 -> 특정 태그들과 충돌한 개체를 하나만 반환
	// CollideCheckWithTagFunc(오브젝트, 태그의 갯수, 태그들)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)
	// * (CBox* _Box, int tagNum, ...) 오버로딩 -> 위와 똑같지만 오브젝트가 아닌 Box를 통째로 넣음
	//   두개 이상의 충돌박스를 가지는 상호작용 오브젝트에 사용
	IDisplayObject * CollideCheckWithTagFunc(IDisplayObject * myObj, int tagNum, ...);
	IDisplayObject * CollideCheckWithTagFunc(CBox* _Box, int tagNum, ...);


	// 충돌 검사 -> 충돌하고 있는 모든 개체들을 vector에 넣어 반환
	vector<IDisplayObject *> CollideCheckAllObject_ReturnVecFunc(IDisplayObject * myObj);

	// 충돌 검사 -> 충돌하고 있는 특정 태그들의 개체들을 vector에 넣어 반환
	// CollideCheckWithTagFunc(오브젝트, 태그의 갯수, 태그들)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)
	vector<IDisplayObject *> CollideCheckWithTag_ReturnVecFunc(IDisplayObject * myObj, int tag, ...);



	// BoundingBox를 Render할 것인지 설정하는 함수
	void SetBoundingBoxRender() { m_BisBoundingBoxRender = !m_BisBoundingBoxRender; }
	bool GetBoundingBoxRender() { return m_BisBoundingBoxRender; }
};


enum OBSTACLE_TAG
{
	PLAYER_TAG,
	ENEMY_TAG,
	BULLET_TAG,				// 총알 Object
	NORMAL_OBSTACLE_TAG,	// 부서지지 않는 일반 Obstacle
	EXPLODE_OBSTACLE_TAG,	// 드럼통 등 폭발하는 Obstacle
	PLAYER_WALL_TAG,		// 사용자가 만드는 벽
	MAP_TAG,				// 맵
	CHECKPOINT_TAG,			// 체크포인트
	CHECKPOINT_LEVER_TAG	// 체크포인트 레버 
};

