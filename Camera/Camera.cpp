#include "../stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	
	m_basePosY = 7.0f;
	m_TransX = 0.0f;
	m_eye = D3DXVECTOR3(0, m_basePosY, -m_distance);
	m_lookAt = &D3DXVECTOR3(0, 0, 0);
	m_up = D3DXVECTOR3(0, 1, 0);
	m_forward = &D3DXVECTOR3(0, 0, 1);
	m_pTarget = NULL;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_Fov = 3.0f;
	m_isRButtonDown = false;

	m_distance = 7.5f;

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
		m_TransX = 3.0f;
	}
	break;
	case WM_RBUTTONUP:
	{
		m_isRButtonDown = false;
		m_Fov = 3.0f;
		m_TransX = 0.0f;
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

	//11,21,31 ->우향벡터
	m_matView._11 = m_right.x;
	m_matView._21 = m_right.y;
	m_matView._31 = m_right.z;
	m_matView._41 = -D3DXVec3Dot(&m_right, &m_eye);
	//12,22,32 ->상향벡터
	m_matView._12 = m_up.x;
	m_matView._22 = m_up.y;
	m_matView._32 = m_up.z;
	m_matView._42 = -D3DXVec3Dot(&m_up, &m_eye);
	//13,23,33->전방벡터
	m_matView._13 = m_forward->x;
	m_matView._23 = m_forward->y;
	m_matView._33 = m_forward->z;
	m_matView._43 = -D3DXVec3Dot(m_forward, &m_eye);
	// 4행 처리
	m_matView._14 = 0;
	m_matView._24 = 0;
	m_matView._34 = 0;
	m_matView._44 = 1;
}

void Camera::Init()
{

	CreateViewmatrix();
	
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
	m_eye = D3DXVECTOR3(m_TransX, m_basePosY, -m_distance);

	D3DXMATRIXA16 RotX, RotY, matRot, TempMove;
	D3DXMatrixRotationY(&RotY, m_rotY);
	matRot = RotX * RotY;

	D3DXVec3TransformNormal(m_forward, &D3DXVECTOR3(0, 0, 1), &RotY);
	D3DXVec3TransformCoord(&m_eye, &m_eye, &RotY);

	m_up = D3DXVECTOR3(0, 1, 0);
	// 전방벡터
	D3DXVec3Normalize(m_forward, m_forward);
	// 우향벡터 (x축)
	D3DXVec3Cross(&m_right, &m_up, m_forward);
	D3DXVec3Normalize(&m_right, &m_right);

	D3DXVec3Normalize(&m_up, &m_up);

	D3DXMatrixRotationAxis(&RotX, &m_right, m_rotX);
	D3DXVec3TransformCoord(&m_up, &m_up, &RotX);
	D3DXVec3TransformCoord(m_forward, m_forward, &RotX);

	m_eye += *m_forward;

	// 타겟이 셋팅되었을 경우
	if (m_pTarget)
	{
		// 타겟(플레이어)의 위치벡터를 설정
		m_lookAt = m_pTarget;
		m_eye = *m_pTarget + m_eye;
	}
	

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	// 오른쪽 마우스 버튼이 눌리면 정조준 모드이므로 우향벡터로 위치벡터를 이동시킨다
	if(g_INPUTMGR->ButtonDown(g_INPUTMGR->RBUTTON))
	{
		m_eye += m_right;
		(*m_lookAt) += m_right;
	}

	CreateViewmatrix();

	D3DXMatrixPerspectiveFovLH(&m_matProj,
		D3DX_PI / m_Fov,
		rc.right / (float)rc.bottom, 1, 1000);

	g_Device->SetTransform(D3DTS_PROJECTION, &m_matProj);
	g_Device->SetTransform(D3DTS_VIEW, &m_matView);
}

