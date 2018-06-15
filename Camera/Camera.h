#pragma once


class Camera
{
private:

	D3DXVECTOR3		m_eye;
	D3DXVECTOR3		*m_lookAt;
	D3DXVECTOR3     *m_forward;
	D3DXVECTOR3		m_up;
	D3DXVECTOR3     m_right;

	D3DXMATRIXA16	m_matView;
	D3DXMATRIXA16	m_matProj;
	D3DXMATRIXA16   matRotX, matRotY, matRot;

	float			m_basePosY; 


	float			m_distance;
	float           m_Fov;
	float			m_rotX;
	float			m_rotY;
	bool			m_isRButtonDown;

	D3DXVECTOR3*	m_pTarget;

	bool                 m_TestDistance;

	POINT			m_ptPrevMouse;

	void CreateViewmatrix();
	


public:
	void Init();
	void Update();
	void SetTarget(D3DXVECTOR3* pTarget) {
		m_pTarget = pTarget; };
	Camera();
	~Camera();
	D3DXVECTOR3 CameraForward()
	{
		return (*m_forward);
	}

	void GetTargetForward(D3DXVECTOR3 * targeforward)
	{
		m_forward = targeforward;
	}
	D3DXVECTOR3 GetCameraUp()
	{
		return m_up;
	}
	D3DXMATRIXA16 GetRotY()
	{
		return matRotY;
	}
	void SetDistance(bool isTrue)
	{
		m_TestDistance = isTrue;
	}


	// 장치 입력을 받아온다.
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

