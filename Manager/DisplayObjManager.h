#pragma once
#include <stdarg.h>
#define g_DisplayObjMGR DisplayObjManager::GetInstance()

class DisplayObjManager
{
	SINGLETON(DisplayObjManager)

private:
	set<IDisplayObject*> m_AllObstacleList;				// ��� ������Ʈ Ȯ��
	map<WORD, vector<IDisplayObject*>> m_ObstacleList;	// Ư�� TAG�� ������Ʈ Ȯ��

	map<string, IDisplayObject*> m_LoadObjList;			// �ε�� .Obj���ϵ��� ������

	bool m_BisBoundingBoxRender;

public:
	// �Ŵ����� ������Ʈ ������ �߰� -> IDisplayObject���� �ڵ� �����
	void AddObject(IDisplayObject* Obj);

	// �� ��ü�� �±׸� �޾Ƽ� �߰�!!! �±װ� �ʿ��� ��ü�� ��� �־�ߵ�
	void AddObjectWithTag(IDisplayObject* Obj, WORD tag);

	void RemoveObject(IDisplayObject* Obj);
	void RemoveObjectWithTag(IDisplayObject* Obj, WORD tag);
	void Destroy();


	// .Obj ������ �̹� �ε�Ǿ����� Ȯ��
	// �ε�� �����̶�� �ѹ� �� �ε��� �ʿ���� ���縸 �ؼ� �ѱ�� �ǹǷ�...
	// ���� ���縦 �ؾߵ��� �������
	IDisplayObject* isObjectLoaded(string str);


	// ------------------------------------------------------------------
	//                  �Ʒ��� [�浹] ���� �Լ����Դϴ�
	// ------------------------------------------------------------------
	
	// �浹�� ��� -> �� ���� ������ ��� ������Ʈ�� STL set���� ��ȯ��
	set<IDisplayObject*> GetAllDisplayObjList() { return m_AllObstacleList; }

	// �±װ� ���Ե� ������Ʈ���� vector�� ��ȯ��
	vector<IDisplayObject*> GetDisplayObjListAtTag(WORD tag) { return m_ObstacleList[tag]; }

	// �浹 �˻� -> �浹�� ��ü �ϳ����� ��ȯ��
	IDisplayObject * CollideCheckAllObjectFunc(IDisplayObject * myObj);


	// �浹 �˻� -> Ư�� �±׵�� �浹�� ��ü�� �ϳ��� ��ȯ
	// CollideCheckWithTagFunc(������Ʈ, �±��� ����, �±׵�)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)
	// * (CBox* _Box, int tagNum, ...) �����ε� -> ���� �Ȱ����� ������Ʈ�� �ƴ� Box�� ��°�� ����
	//   �ΰ� �̻��� �浹�ڽ��� ������ ��ȣ�ۿ� ������Ʈ�� ���
	IDisplayObject * CollideCheckWithTagFunc(IDisplayObject * myObj, int tagNum, ...);
	IDisplayObject * CollideCheckWithTagFunc(CBox* _Box, int tagNum, ...);


	// �浹 �˻� -> �浹�ϰ� �ִ� ��� ��ü���� vector�� �־� ��ȯ
	vector<IDisplayObject *> CollideCheckAllObject_ReturnVecFunc(IDisplayObject * myObj);

	// �浹 �˻� -> �浹�ϰ� �ִ� Ư�� �±׵��� ��ü���� vector�� �־� ��ȯ
	// CollideCheckWithTagFunc(������Ʈ, �±��� ����, �±׵�)
	// ex) CollideCheckWithTagFunc(obj, 3, ENEMY_TAG, BULLET_TAG, NORMAL_OBSTACLE_TAG)
	vector<IDisplayObject *> CollideCheckWithTag_ReturnVecFunc(IDisplayObject * myObj, int tag, ...);



	// BoundingBox�� Render�� ������ �����ϴ� �Լ�
	void SetBoundingBoxRender() { m_BisBoundingBoxRender = !m_BisBoundingBoxRender; }
	bool GetBoundingBoxRender() { return m_BisBoundingBoxRender; }
};


enum OBSTACLE_TAG
{
	PLAYER_TAG,
	ENEMY_TAG,
	BULLET_TAG,				// �Ѿ� Object
	NORMAL_OBSTACLE_TAG,	// �μ����� �ʴ� �Ϲ� Obstacle
	EXPLODE_OBSTACLE_TAG,	// �巳�� �� �����ϴ� Obstacle
	PLAYER_WALL_TAG,		// ����ڰ� ����� ��
	MAP_TAG,				// ��
	CHECKPOINT_TAG,			// üũ����Ʈ
	CHECKPOINT_LEVER_TAG	// üũ����Ʈ ���� 
};

