#pragma once
#include "../Base/IDisplayObject.h"

class BDrawingGroup;

class DMapObstacle :	public IDisplayObject
{
private:
	// DrawingGroup, Mesh, MTLTex
	vector<BDrawingGroup*>	m_vecDrawingGroup;
	LPD3DXMESH				m_pMesh;
	vector<MTLTEX*>			m_vecMtlTex;

public:
	DMapObstacle();
	~DMapObstacle();

	// map에서 불러온 Obstacle을 setting한다
	// 즉 x y z filename.obj를 받았을 때에 new를 해서 새로 obstacle 클래스를 만든 후
	// setObjstacleOnMap을 실행시키고 Obstacle매니저에 넣으면 된다 
	void SetObstacle(const char *fileName, D3DXVECTOR3 WorldPos);
	
	void RenderDrawingGroup();
	void RenderMesh();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

