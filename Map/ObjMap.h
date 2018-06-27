#pragma once
#include "IMap.h"

// Obj를 이용해서 맵을 불러오는 클래스
// 현재 4개가 Release가 안되는 문제가 있다
class ObjMap : public IMap
{
private:
	LPD3DXMESH				m_pMeshMap;
	vector<MTLTEX*>			m_vecMtlTex;

	float					m_rayOffsetY;	// GetHeight를 계산할때 ray를 쏘는 Y축의 높이

	string					m_fileName;

	// 맵의 surface!
	vector<D3DXVECTOR3>		m_vecSurface;
	bool					m_BisRenderSurface;

public:
	ObjMap();
	~ObjMap();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual bool GetHeight(OUT float & height, const D3DXVECTOR3 & pos) override;
	virtual bool CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY) { return true; }

	void RenderMesh();
	void RenderSurface();

	void SetObjfileName(string s) { m_fileName = s; }
};
