#include "../stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	m_basePosY = 5.0f;

	m_eye = D3DXVECTOR3(0, m_basePosY, 3.0f);
	m_lookAt = &D3DXVECTOR3(0, 0, 0);
	m_up = D3DXVECTOR3(0, 1, 0);
	m_forward = &D3DXVECTOR3(0, 0, 1);
	m_pTarget = NULL;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_Fov = 3.0f;
	m_isRButtonDown = false;

	m_distance = 3.0f;
	m_TestDistance = false;

}


Camera::~Camera()
{
}

void Camera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RBUTTONDOWN:
	{
		m_isRButtonDown = true;
		m_Fov = 6.0f;
	}
	break;
	case WM_RBUTTONUP:
	{
		m_isRButtonDown = false;
		m_Fov = 3.0f;
	}
	break;
	case WM_MOUSEMOVE:
	{
		POINT currPoint;
		currPoint.x = LOWORD(lParam);
		currPoint.y = HIWORD(lParam);

		m_rotY += (currPoint.x - m_ptPrevMouse.x) / 100.0f;
		m_rotX += (currPoint.y - m_ptPrevMouse.y) / 100.0f;


		if (m_rotX <= -D3DX_PI * 0.5f + D3DX_16F_EPSILON)
		{
			m_rotX = -D3DX_PI * 0.5f;
		}
		if (m_rotX >= D3DX_PI * 0.3f - D3DX_16F_EPSILON)
		{
			m_rotX = D3DX_PI * 0.3f;
		}
		if (m_rotY < -D3DX_PI * 3.0f + D3DX_16F_EPSILON)
		{
			m_rotY = -D3DX_PI * 3.0f;
		}
		if (m_rotY > D3DX_PI * 3.0f - D3DX_16F_EPSILON)
		{
			m_rotY = D3DX_PI * 3.0f;
		}
		m_ptPrevMouse = currPoint;
	}
	break;
	case WM_MOUSEWHEEL:
		if (m_TestDistance)
		{
			m_distance -= GET_WHEEL_DELTA_WPARAM(wParam) / 50.0f;
			if (m_distance <= 10) m_distance = 10;
			if (m_distance >= 30) m_distance = 30;
		}
		break;
	}

}

void Camera::CreateViewmatrix()
{
	
	// Àü¹æº¤ÅÍ
	D3DXVec3Normalize(m_forward, m_forward);
	// ¿ìÇâº¤ÅÍ (xÃà)
	D3DXVec3Cross(&m_right, &m_up, m_forward);
	D3DXVec3Normalize(&m_right, &m_right);
	m_up = D3DXVECTOR3(0, 1, 0);
	

	//11,21,31 ->¿ìÇâº¤ÅÍ
	m_matView._11 = m_right.x;
	m_matView._21 = m_right.y;
	m_matView._31 = m_right.z;
	m_matView._41 = -D3DXVec3Dot(&m_right, &m_eye);
	//12,22,32 ->»óÇâº¤ÅÍ
	m_matView._12 = m_up.x;
	m_matView._22 = m_up.y;
	m_matView._32 = m_up.z;
	m_matView._42 = -D3DXVec3Dot(&m_up, &m_eye);
	//13,23,33->Àü¹æº¤ÅÍ
	m_matView._13 = m_forward->x;
	m_matView._23 = m_forward->y;
	m_matView._33 = m_forward->z;
	m_matView._43 = -D3DXVec3Dot(m_forward, &m_eye);
	// 4Çà Ã³¸®
	m_matView._14 = 0;
	m_matView._24 = 0;
	m_matView._34 = 0;
	m_matView._44 = 1;
}

void Camera::Init()
{
	/*
	if (m_pTarget)
	{
		m_lookAt = m_pTarget;
	}

	CreateViewmatrix();
	*/
	D3DXMatrixLookAtLH(&m_matView,
		&m_eye, m_lookAt, &m_up);
	g_Device->SetTransform(D3DTS_VIEW, &m_matView);


	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMatrixPerspectiveFovLH(&m_matProj,
		D3DX_PI / m_Fov,
		rc.right / (float)rc.bottom, 1, 1000);
	g_Device->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void Camera::Update()
{
	m_eye = D3DXVECTOR3(0, m_basePosY, -m_distance);

	D3DXMatrixRotationX(&matRotX, m_rotX);
	D3DXMatrixRotationY(&matRotY, m_rotY);

	matRot = matRotX * matRotY;

	D3DXVec3TransformCoord(&m_eye, &m_eye, &matRot);

	if (m_pTarget)
	{
		m_lookAt = m_pTarget;
		m_eye = *m_pTarget + m_eye;
	}

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMatrixLookAtLH(&m_matView,
		&m_eye, m_lookAt, &m_up);
	//CreateViewmatrix();

	D3DXMatrixPerspectiveFovLH(&m_matProj,
		D3DX_PI / m_Fov,
		rc.right / (float)rc.bottom, 1, 1000);

	g_Device->SetTransform(D3DTS_PROJECTION, &m_matProj);
	g_Device->SetTransform(D3DTS_VIEW, &m_matView);
}

