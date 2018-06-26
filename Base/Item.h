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
	SINGLESHOT, // 저격
	SHOT, // 샷건
	AUTOMATICFIRE,  // 라이플
};

struct ITEMINFO
{
	int HP;
	int Attack;
	int Defence;
	float RateTime; // 치료 아이템일 경우 아이템을 사용하는데 걸리는 시간을 잰다.
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
	// 현재 아이템의 타입 확인
	int GetItemType()
	{
		return INFO.ItemType;
	}
	// 아이템 타입(총)이 확인됀 총기 종류 확인
	int GetFireType()
	{
		return INFO.FireType;
	}

	void LoadMesh(const char* Path, const char* fileName,float ScaleXYZ = 1.0f);

	// 부모(캐릭터)의 월드 행렬을 셋팅한다.
	void SetParantWM(D3DXMATRIXA16 *ParantWM)
	{
		m_ParentMat = ParantWM;
	}
	void SetParantWM2(D3DXMATRIXA16 *ParantWM)
	{
		m_ParentMat2 = ParantWM;
	}

	// 부모(캐릭터)의 방향을 지정한다.
	void SetCharacterRotationY(float rotY)
	{
		m_rotY = rotY;
	}
	



	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};