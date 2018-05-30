#pragma once
#include "IMap.h"

class MHeightMap : public IMap
{
protected:
	MTLTEX *	m_pMtlTex;
	LPD3DXMESH	m_pMesh;

	vector<D3DXVECTOR3> m_vecSurfaceVertex;

	D3DXVECTOR3 m_charPos;


public:
	MHeightMap();
	virtual ~MHeightMap();

	// raw파일에서 HeightMap을 불러오는 메서드
	// fullPath = 파일 이름을 포함한 경로
	void Load(const char* fullPath, D3DXMATRIXA16* pMat = NULL);

	// 메테리얼과 텍스쳐를 설정하는 메서드
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

