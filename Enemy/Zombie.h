#pragma once
class Player;

enum FSM_STATE
{
	STATE_MOVE,
	STATE_ATTACK
};
class Zombie : public IDisplayObject
{
private:
	vector<VERTEX_PC>		m_vecVertex;
	vector<WORD>			m_vecIndex;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	D3DVERTEXBUFFER_DESC	m_VBDesc;
	D3DINDEXBUFFER_DESC		m_IBDesc;

	D3DXVECTOR3				m_Destination;

	float					m_moveSpeed;
	vector<Player*>			m_pPlayer;
	int						m_state;

	vector<D3DXVECTOR3> vecPos;

	D3DCOLOR red = D3DCOLOR_XRGB(255, 0, 0);
	D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);
	D3DCOLOR blue = D3DCOLOR_XRGB(0, 0, 255);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);
	D3DCOLOR yellow = D3DCOLOR_XRGB(255, 255, 0);

public:
	Zombie();
	~Zombie();

	void Init();
	void Update();
	void Render();


	void CreateBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb,
		vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex);

	void SetVertex(vector<VERTEX_PC> &vecVertexOut,
		vector<WORD> &vecIndexOut, vector<D3DXVECTOR3> vecPos, D3DCOLOR color);

	void SetBuffer(LPDIRECT3DVERTEXBUFFER9 &pVb,
		LPDIRECT3DINDEXBUFFER9 &pIb,
		vector<VERTEX_PC> &vecVertex, vector<WORD> &vecIndex);
};
