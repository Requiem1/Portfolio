#pragma once
#include "IMap.h"

// Obj�� �̿��ؼ� ���� �ҷ����� Ŭ����
// ���� 4���� Release�� �ȵǴ� ������ �ִ�
class ObjMap : public IMap
{
private:
	LPD3DXMESH				m_pMeshMap;
	vector<MTLTEX*>			m_vecMtlTex;

	float					m_rayOffsetY;	// GetHeight�� ����Ҷ� ray�� ��� Y���� ����

	string					m_fileName;

	// ���� surface!
	vector<D3DXVECTOR3>		m_vecSurface;
	bool					m_BisRenderSurface;

public:
	ObjMap();
	~ObjMap();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual bool GetHeight(OUT float & height, const D3DXVECTOR3 & pos) override;
	virtual bool CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY) { return true; }

	void RenderMesh();
	void RenderSurface();

	void SetObjfileName(string s) { m_fileName = s; }
};
