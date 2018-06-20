#pragma once
#include "IMap.h"

class BDrawingGroup;

class ObjMap : public IMap
{
private:
	vector<BDrawingGroup*>	m_vecDrawingGroup;
	LPD3DXMESH				m_pMeshMap;
	vector<MTLTEX*>			m_vecMtlTex;

	float					m_rayOffsetY;

	string					m_fileName;

	// ∏ ¿« surface!
	vector<D3DXVECTOR3>		m_vecSurface;

public:
	ObjMap();
	~ObjMap();

	// IDisplayObject¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual bool GetHeight(OUT float & height, const D3DXVECTOR3 & pos) override;
	virtual bool CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY) { return true; }

	void RenderDrawingGroup();
	void RenderMesh();
	void RenderSurface();

	void SetObjfileName(string s) { m_fileName = s; }
};

