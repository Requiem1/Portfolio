#pragma once
#include "../stdafx.h"
#include "../Base/IDisplayObject.h"

//class AStar;

class DMap : public IDisplayObject
{
protected:
	vector<D3DXVECTOR3> m_vecVertex;
	size_t				m_numTile;
	size_t				m_dimension;

	D3DXVECTOR3			m_size;
	D3DXVECTOR3			m_scale;

	//AStar*				m_pAstar;

	DMap() {}	//{m_pAstar = NULL; }
	
public:
	virtual ~DMap() {}

	virtual bool GetHeight(OUT float &height, const D3DXVECTOR3 &pos) = 0;
	virtual bool CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY) = 0;

	const vector<D3DXVECTOR3>& GetVertex() { return m_vecVertex; }

	void Setdimension(size_t dimension)
	{
		m_dimension = dimension;
		m_numTile = m_dimension - 1;
	}

	const D3DXVECTOR3& GetSize() { return m_size; }

	//AStar* GetAstar() { return m_pAstar; }
};