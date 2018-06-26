#pragma once

#include "IDisplayObject.h"


class ObjLoader;

enum ITEMTYPE
{
	HEALING,
	SHOOT,
	BULLET,
	AMOR
};

enum FIRETYPE
{
	SINGLESHOT, // ����
	SHOT, // ����
	AUTOMATICFIRE,  // ������
};

struct ITEMINFO
{
	int HP;
	int Attack;
	int Defence;
	float RateTime; // ġ�� �������� ��� �������� ����ϴµ� �ɸ��� �ð��� ���.
	FIRETYPE FireType;
	ITEMTYPE ItemType;
};

class DITEM : public IDisplayObject
{
protected:
	LPD3DXMESH              m_Mesh;
	vector<MTLTEX*>			m_vecMtlTex;

	D3DXMATRIXA16           *m_ParentMat;
	D3DXMATRIXA16           *m_ParentMat2;
	          
	D3DXMATRIXA16           m_CTransMat;
	D3DXVECTOR3             m_forward;
	D3DXMATRIXA16           m_CSMat;


	D3DXMATRIXA16           matBaseR;



	FIRETYPE                m_fireType;

	bool                    m_isClick;
	float                   m_rotY;
	float                   m_scale;
	float			        m_moveSpeed;
	float			        m_currMoveSpeedRate;

	ITEMINFO INFO;

public:
	DITEM();
	~DITEM();


	void SetClick(bool _Click)
	{
		m_isClick = _Click;
	}


	void SetForward(D3DXVECTOR3 _Forward)
	{
		m_forward = _Forward;
	}
	// ���� �������� Ÿ�� Ȯ��
	int GetItemType()
	{
		return INFO.ItemType;
	}
	// ������ Ÿ��(��)�� Ȯ�Ή� �ѱ� ���� Ȯ��
	int GetFireType()
	{
		return INFO.FireType;
	}

	void LoadMesh(const char* Path, const char* fileName,float ScaleXYZ = 1.0f);

	// �θ�(ĳ����)�� ���� ����� �����Ѵ�.
	void SetParantWM(D3DXMATRIXA16 *ParantWM)
	{
		m_ParentMat = ParantWM;
	}
	void SetParantWM2(D3DXMATRIXA16 *ParantWM)
	{
		m_ParentMat2 = ParantWM;
	}

	// �θ�(ĳ����)�� ������ �����Ѵ�.
	void SetCharacterRotationY(float rotY)
	{
		m_rotY = rotY;
	}
	



	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};