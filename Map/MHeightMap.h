#pragma once
#include "IMap.h"

class IHeightMap : public DMap
{
protected:
	MTLTEX *	m_pMtlTex;
	LPD3DXMESH	m_pMesh;

	vector<D3DXVECTOR3> m_vecSurfaceVertex;

	D3DXVECTOR3 m_charPos;


public:
	IHeightMap();
	virtual ~IHeightMap();

	void Load(const char* fullPath, D3DXMATRIXA16* pMat = NULL);
	void SetMtlTex(D3DMATERIAL9 &mtl, LPDIRECT3DTEXTURE9 pTex);

	// IMap을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual bool GetHeight(OUT float & height, const D3DXVECTOR3 & pos) override;
	virtual bool CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY) override;

	// picking 체크 구역을 5x5정도로 나눠서 그 위치로 이동하는 함수
	void SetSurface();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

