
#include "../stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
	for (map<string, IDisplayObject*>::iterator it = m_pMapItem.begin(); it != m_pMapItem.end(); it++)
		it->second->ReleaseAll();
}

void Inventory::Init()
{
}

void Inventory::Update()
{
}
