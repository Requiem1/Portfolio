#pragma once
#include "../stdafx.h"
#include "../Base/IDisplayObject.h"

//class AStar;

// Map의 인터페이스 - 맵의 버텍스, 크기 들을 Protected로 상속받음
class IMap : public IDisplayObject
{
protected:
	vector<D3DXVECTOR3> m_vecVertex;
	size_t				m_numTile;
	size_t				m_dimension;

	D3DXVECTOR3			m_size;
	D3DXVECTOR3			m_scale;

	//AStar*				m_pAstar;

	// 여러개의 맵을 로드해올 때의 mapIndex;
	size_t				m_mapIndex;

	IMap() {}	//{m_pAstar = NULL; }
	
public:
	virtual ~IMap() {}

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