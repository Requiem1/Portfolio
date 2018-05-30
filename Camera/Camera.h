#pragma once

#define g_pCamera Camera::GetInstance()

class Camera
{
	SINGLETON(Camera)
private:
	D3DXVECTOR3		m_eye;
	D3DXVECTOR3		m_lookAt;
	D3DXVECTOR3		m_up;
	D3DXMATRIXA16	m_matView;
	D3DXMATRIXA16	m_matProj;
	D3DXMATRIXA16   matRotX, matRotY, matRot;

	float			m_basePosY;
	float			m_distance;
	float           m_Fov;
	float			m_rotX;
	float			m_rotY;
	bool			m_isLbuttonDown;
	bool            m_TestDistance;

	POINT			m_ptPrevMouse;
	D3DXVECTOR3*	m_pTarget;

	


public:
	void Init();
	void Update();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetTarget(D3DXVECTOR3* pTarget) { m_pTarget = pTarget; };

	void SetTest(bool T)
	{
		m_TestDistance = T;
	}


	D3DXVECTOR3 GetCameraPos()
	{
		return m_eye;
	}
	D3DXMATRIXA16 GetRot()
	{
		return matRot;
	}
	D3DXMATRIXA16 GetCameraRotY()
	{
		return matRotY;
	}
	D3DXVECTOR3 GetCameraUp()
	{
		return m_up;
	}
};

