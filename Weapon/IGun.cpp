#include "../stdafx.h"
#include "IGun.h"
//#include "Bullet.h"

IGun::IGun()
{
}

IGun::~IGun()
{
}

void IGun::SetBullet(OUT D3DXVECTOR3 *BulletForward)
{
	/*
	if (m_fireType == IBullet::SHOTGUNBULLET)
	{
		D3DXVECTOR3 Change = *m_forward;
		D3DXMATRIXA16 XRot, YRot, Mat;

		float XRadian, YRadian;
		XRadian = GetRandomFloat(-0.3, 0.3);
		YRadian = GetRandomFloat(-0.3, 0.3);


		D3DXMatrixRotationX(&XRot, XRadian);
		D3DXMatrixRotationY(&YRot, YRadian);

		Mat = XRot + YRot;

		D3DXVec3TransformNormal(BulletForward, &Change, &Mat);
	}
	else
		(*BulletForward) = *m_forward;
	*/
	

}


