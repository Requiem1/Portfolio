#pragma once

#include "../Base/Item.h"

//class IBullet;

class IGun : public DITEM
{
protected:
	int CurrentSize;
	int FullSize;
	//vector<IBullet*> m_vecBullet;



public:
	IGun();
	~IGun();

	

	// 현재 잔여 총알을 확인한다.
	// 예 : 현재총알/최대총알 형태일때 현재 총알의 갯수를 리턴
	int GetCurrentBulletSize()
	{
		return CurrentSize;
	}
	// 최대 총알 갯수를 확인한다.
	int GetFullBulletSize()
	{
		return FullSize;
	}
	// 발사됄 총알의 발사 타입을 지정한다.
	void SetType(FIRETYPE T)
	{
		m_fireType = T;
	}
	

	// 발사됄 총알의 방향을 지정한다.
	void SetBullet(OUT D3DXVECTOR3* BulletForward);

	// DITEM을(를) 통해 상속됨
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;


};
