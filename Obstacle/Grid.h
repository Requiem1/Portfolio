#pragma once
#include "../Base/BaseObject.h"
class Grid :
	public BaseObject
{
public:
	vector<VERTEX_PC> m_vecVertex;

	Grid();
	~Grid();

	void Init();
	void Render();
};

