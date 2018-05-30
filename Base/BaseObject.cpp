#include "../stdafx.h"
#include "BaseObject.h"


BaseObject::BaseObject()
	:m_refCount(1)
{
}

BaseObject::~BaseObject()
{
	assert(m_refCount <= 0 && "생성됀 오브젝트 중에 아직 해제돼지 않은 오브젝트가 있습니다. Release를 이용해 해제해야 합니다.");
}

void BaseObject::AddRef()
{
	m_refCount++;
}

void BaseObject::Release()
{
	m_refCount--;
	if (m_refCount <= 0)
	{
		delete this;
	}
}
