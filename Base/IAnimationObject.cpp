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
