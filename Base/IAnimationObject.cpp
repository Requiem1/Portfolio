#include "../stdafx.h"
#include "IAnimationObject.h"



IAnimationObject::IAnimationObject()
{
}


IAnimationObject::~IAnimationObject()
{
}

void IAnimationObject::SetXfilePath(CString filePath, CString fileName)
{
	m_filePath = filePath;
	m_fileName = fileName;
}



// Ȯ�ο�!
void IAnimationObject::PrintBoneList()
{
	for (auto p : m_vecBonelist)
	{
		if (p.second->Name != NULL)
			cout << p.second->Name << ' ';
	}
}