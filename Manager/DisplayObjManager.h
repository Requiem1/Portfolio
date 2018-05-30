#pragma once
#include <stdarg.h>
#define g_DisplayObjMGR DisplayObjManager::GetInstance()

class DisplayObjManager
{
	SINGLETON(DisplayObjManager)

private:
	set<IDisplayObject*> m_AllObstacleList;				// ��� ������Ʈ Ȯ��
	map<WORD, vector<IDisplayObject*>> m_ObstacleList;	// Ư�� TAG�� ������Ʈ Ȯ��

	bool m_isBoundingBoxRender;

public:

	void AddObject(IDisplayObject* Obj);
	void AddObjectWithTag(IDisplayObject* Obj, WORD tag);

	void RemoveObject(IDisplayObject* Obj);
	void RemoveObjectWithTag(IDisplayObject* Obj, WORD tag);
	void Destroy();

	// �浹�� ��� -> �� ���� ������ ��� ������Ʈ�� STL set���� ��ȯ��
	set<IDisplayObject*> GetAllDisplayObjList() { return m_AllObstacleList; }

	// �±װ� ���Ե� ������Ʈ���� vector�� ��ȯ��
	vector<IDisplayObject*> GetDisplayObjListAtTag(WORD tag) { return m_ObstacleList[tag]; }

	// �浹 �˻� -> �浹�� ��ü �ϳ����� ��ȯ��
	IDisplayObject * CollideCheckAllObjectFunc(IDisplayObject * myObj);

	// �浹 �˻� -> Ư�� �±׵�� �浹�� ��ü�� �ϳ��� ��ȯ
	// CollideCheckWithTagFunc(������Ʈ, �±��� ����, �±׵�)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)
	IDisplayObject * CollideCheckWithTagFunc(IDisplayObject * myObj, int tagNum, ...);

	// �浹 �˻� -> �浹�ϰ� �ִ� ��� ��ü���� vector�� �־� ��ȯ
	vector<IDisplayObject *> CollideCheckAllObject_ReturnVecFunc(IDisplayObject * myObj);

	// �浹 �˻� -> �浹�ϰ� �ִ� Ư�� �±׵��� ��ü���� vector�� �־� ��ȯ
	// CollideCheckWithTagFunc(������Ʈ, �±��� ����, �±׵�)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)
	vector<IDisplayObject *> CollideCheckWithTag_ReturnVecFunc(IDisplayObject * myObj, int tag, ...);

	void SetBoundingBoxRender() { m_isBoundingBoxRender = !m_isBoundingBoxRender; }
	bool GetBoundingBoxRender() { return m_isBoundingBoxRender; }
};


enum OBSTACLE_TAG
{
	PLAYER_TAG,
	ENEMY_TAG,
	BULLET_TAG,				// �Ѿ� Object
	NORMAL_OBSTACLE_TAG,	// �μ����� �ʴ� �Ϲ� Obstacle
	EXPLODE_OBSTACLE_TAG,	// �巳�� �� �����ϴ� Obstacle
	PLAYER_WALL_TAG,		// ����ڰ� ����� ��
	MAP_TAG					// ��
};

