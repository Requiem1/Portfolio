#include "../stdafx.h"
#include "IDisplayObject.h"

IDisplayObject::IDisplayObject()
{
	m_pParent = NULL;
	D3DXMatrixIdentity(&m_matWorld);

	m_BoundingBox = new CBox();

	// 맵의 경우 충돌감지를 하지 않게 RemoveObject를 해야한다!
	// 이건 일반 오브젝트이므로 AddObject를 함
	g_DisplayObjMGR->AddObject(this);
}


IDisplayObject::~IDisplayObject()
{
	SAFE_DELETE(m_BoundingBox);
}

void IDisplayObject::AddChild(IDisplayObject * pChild)
{
	if (pChild == NULL) return;
	m_vecPChild.push_back(pChild);
	pChild->m_pParent = this;
}

void IDisplayObject::ReleaseAll()
{
	for (auto child : m_vecPChild)
		if (child) child->ReleaseAll();

	Release();
}