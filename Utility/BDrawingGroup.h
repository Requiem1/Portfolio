#pragma once

class BDrawingGroup :
	public BaseObject
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	size_t					m_numTri;

	MTLTEX*					m_MtlTex;

public:
	BDrawingGroup();
	~BDrawingGroup();

	void SetMtlTex(MTLTEX* p) { m_MtlTex = p; }
	void SetVertexBuffer(const vector<VERTEX_PNT> & vec);
	void Render();
};

