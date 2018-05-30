#pragma once
#include "../Base/IDisplayObject.h"

class Inventory
{
private:
	map<string, IDisplayObject*> m_pMapItem;
public:
	Inventory();
	~Inventory();

	void Init();
	void Update();

	void SetItem(string name, IDisplayObject* Item)
	{
		m_pMapItem[name] = Item;
	}
	IDisplayObject* GetItem(string name)
	{
		if (m_pMapItem[name] == NULL)
			return NULL;

		return m_pMapItem[name];
	}
	
};

