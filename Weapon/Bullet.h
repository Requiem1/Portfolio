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

	// DITEM��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	// Ŭ���Ұ�� ����
	void SetClick(bool IsClick)
	{
		m_BClick = IsClick;
	}
	bool GetClick()
	{
		return m_BClick;
	}
	// ���ؽ� ���� ���� (�ε� ���� �ϼ��Ÿ� ������� ����)
	void VertexBuffer(LPDIRECT3DVERTEXBUFFER9 & pVb, LPDIRECT3DINDEXBUFFER9 & pIb, vector<VERTEX_PC>& vecVertex, vector<WORD>& vecIndex);

	// �Ѿ��� ���溤�� ��������
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
	// �Ѿ� Ÿ���� �����Ѵ�.
	// ������,����,����,���ݿ�
	void SetBulletType(BULLETTYPE B)
	{
		m_EBulletType = B;
	}
	// �Ѿ��� ��ġ�� ������ �������ش�.
	void SetPos_Direction(D3DXVECTOR3 ParantPos, D3DXVECTOR3 ParantDirection)
	{
		m_pos = ParantPos;
		m_forward = ParantDirection;
	}



};