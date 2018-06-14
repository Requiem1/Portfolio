#include "../stdafx.h"
#include "IAnimationObject.h"



IAnimationObject::IAnimationObject()
{
}


IAnimationObject::~IAnimationObject()
{
}

void IAnimationObject::SetXfile(CString filePath, CString fileName, float ScaleXYZ)
{
	m_filePath = filePath;
	m_fileName = fileName;

	// ũ�⸦ ��������
	SetScale(ScaleXYZ);
	m_pos.x *= ScaleXYZ;
	m_pos.y *= ScaleXYZ;
	m_pos.z *= ScaleXYZ;
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