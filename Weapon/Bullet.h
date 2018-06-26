#pragma once

#include "../Base/Item.h"



class IBullet :public DITEM
{
private:
	bool m_BClick;

	vector<VERTEX_PC>		m_vecVertex;
	vector<WORD>			m_vecIndex;


	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	D3DVERTEXBUFFER_DESC	m_VBDesc;
	D3DINDEXBUFFER_DESC		m_IBDesc;

public:
	
	enum BULLETTYPE
	{
		RIFLEBULLET,
		SHOTGUNBULLET,
		PISTOLBULLET,
		SNIPERBULLET
	};
	IBullet();
	~IBullet();

	BULLETTYPE m_EBulletType;

	// DITEM을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	// 클릭할경우 설정
	void SetClick(bool IsClick)
	{
		m_BClick = IsClick;
	}
	bool GetClick()
	{
		return m_BClick;
	}
	// 버텍스 버퍼 설정 (로딩 툴이 완성돼면 사용하지 않음)
	void VertexBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex);

	// 총알의 전방벡터 가져오기
	D3DXVECTOR3 GetForward()
	{
		return m_forward;
	}
	void SetStartBulletPos(D3DXVECTOR3 _pos)
	{
		m_pos = _pos;
	}
	D3DXVECTOR3 GetPos()
	{
		return m_pos;
	}
	// 총알 타입을 지정한다.
	// 라이플,샷건,권총,저격용
	void SetBulletType(BULLETTYPE B)
	{
		m_EBulletType = B;
	}
	// 총알의 위치와 방향을 지정해준다.
	void SetPos_Direction(D3DXVECTOR3 ParantPos, D3DXVECTOR3 ParantDirection)
	{
		m_pos = ParantPos;
		m_forward = ParantDirection;
	}



};