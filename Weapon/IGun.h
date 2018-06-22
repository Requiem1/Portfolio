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

	

	// ���� �ܿ� �Ѿ��� Ȯ���Ѵ�.
	// �� : �����Ѿ�/�ִ��Ѿ� �����϶� ���� �Ѿ��� ������ ����
	int GetCurrentBulletSize()
	{
		return CurrentSize;
	}
	// �ִ� �Ѿ� ������ Ȯ���Ѵ�.
	int GetFullBulletSize()
	{
		return FullSize;
	}
	// �߻�� �Ѿ��� �߻� Ÿ���� �����Ѵ�.
	void SetType(FIRETYPE T)
	{
		m_fireType = T;
	}
	

	// �߻�� �Ѿ��� ������ �����Ѵ�.
	void SetBullet(OUT D3DXVECTOR3* BulletForward);

	// DITEM��(��) ���� ��ӵ�
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;


};
