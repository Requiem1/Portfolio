#include "../stdafx.h"
#include "DisplayObjManager.h"


DisplayObjManager::DisplayObjManager()
{
	m_BisBoundingBoxRender = false;
}


DisplayObjManager::~DisplayObjManager()
{
	Destroy();
}

void DisplayObjManager::AddObject(IDisplayObject * Obj)
{
	m_AllObstacleList.insert(Obj);
}

void DisplayObjManager::AddObjectWithTag(IDisplayObject * Obj, WORD tag)
{
	m_ObstacleList[tag].push_back(Obj);
}

void DisplayObjManager::RemoveObject(IDisplayObject * Obj)
{
	m_AllObstacleList.erase(Obj);
}

void DisplayObjManager::RemoveObjectWithTag(IDisplayObject * Obj, WORD tag)
{
	vector<IDisplayObject*>::iterator iter;
	iter = find(m_ObstacleList[tag].begin(), m_ObstacleList[tag].end(), Obj);

	m_ObstacleList[tag].erase(iter);
}

void DisplayObjManager::Destroy()
{
	m_AllObstacleList.clear();
	m_ObstacleList.clear();
}

IDisplayObject * DisplayObjManager::isObjectLoaded(string str)
{
	// 파일이 있다면 그 파일의 오브젝트를 반환
	if (m_LoadObjList.find(str) != m_LoadObjList.end())
	{
		
	}

	// 없으면 null 반환
	return nullptr;
}

IDisplayObject * DisplayObjManager::CollideCheckAllObjectFunc(IDisplayObject * myObj)
{
	// 바운싱 박스 충돌 검사!
	// BSP를 이용한 공간분할로 충돌체크를 해야되지만
	// 일단은 objManager의 객체들과 모두 충돌체크를 돌리도록 한다
	for (auto p : m_AllObstacleList)
	{
		if (p == myObj)
			continue;
	
		// Collision Detection Test!
		int nRet = BoxBoxIntersectionTest(*(myObj->GetCBox()), *(p->GetCBox()));
	
		// 가장 처음 충돌한 하나의 값만 반환한다
		if (nRet == 1)
		{
			return p;
		}
	}
	
	return NULL;
}

IDisplayObject * DisplayObjManager::CollideCheckWithTagFunc(IDisplayObject * myObj, int tagNum, ...)
{
	// 가변인자를 이용하여 여러개의 Tag를 받는다
	va_list argList;
	va_start(argList, tagNum);

	int EnowTag = va_arg(argList, int);

	// 바운싱 박스 충돌 검사!
	// BSP를 이용한 공간분할로 충돌체크를 해야되지만
	// 일단은 objManager의 객체들과 모두 충돌체크를 돌리도록 한다
	for (auto p : m_ObstacleList[EnowTag])
	{
		if (p == myObj)
			continue;

		// Collision Detection Test!
		int nRet = BoxBoxIntersectionTest(*(myObj->GetCBox()), *(p->GetCBox()));

		// 가장 처음 충돌한 하나의 값만 반환한다
		if (nRet == 1)
		{
			va_end(argList);
			return p;
		}
	}

	va_end(argList);

	return NULL;
}


vector<IDisplayObject *> DisplayObjManager::CollideCheckAllObject_ReturnVecFunc(IDisplayObject * myObj)
{
	vector<IDisplayObject *> vec;

	// 바운싱 박스 충돌 검사!
	// BSP를 이용한 공간분할로 충돌체크를 해야되지만
	// 일단은 objManager의 객체들과 모두 충돌체크를 돌리도록 한다
	for (auto p : m_AllObstacleList)
	{
		if (p == myObj)
			continue;

		// Collision Detection Test!
		int nRet = BoxBoxIntersectionTest(*(myObj->GetCBox()), *(p->GetCBox()));

		// 충돌한 객체들을 vecter에 삽입한다
		if (nRet == 1)
		{
			vec.push_back(p);
		}
	}

	// 벡터를 반환한다
	return vec;
}

vector<IDisplayObject *> DisplayObjManager::CollideCheckWithTag_ReturnVecFunc(IDisplayObject * myObj, int tag, ...)
{
	vector<IDisplayObject *> vec;

	// 가변인자를 이용하여 여러개의 Tag를 받는다
	va_list argList;
	va_start(argList, tag);

	// 바운싱 박스 충돌 검사!
	// BSP를 이용한 공간분할로 충돌체크를 해야되지만
	// 일단은 objManager의 객체들과 모두 충돌체크를 돌리도록 한다
	for (auto p : m_ObstacleList[va_arg(argList, int)])
	{
		if (p == myObj)
			continue;

		// Collision Detection Test!
		int nRet = BoxBoxIntersectionTest(*(myObj->GetCBox()), *(p->GetCBox()));

		// 충돌한 객체들을 vecter에 삽입한다
		if (nRet == 1)
		{
			vec.push_back(p);
		}
	}

	va_end(argList);

	// 벡터를 반환한다
	return vec;
}
