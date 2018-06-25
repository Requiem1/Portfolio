#pragma once


class Camera
{
private:

	D3DXVECTOR3		m_eye;
	D3DXVECTOR3		*m_lookAt;
	D3DXVECTOR3     *m_forward;
	D3DXVECTOR3		m_up;
	D3DXVECTOR3     m_right;
	D3DXVECTOR3     *m_characterMatrix;

	D3DXMATRIXA16   m_matFinal;
	D3DXMATRIXA16	m_matView;
	D3DXMATRIXA16	m_matProj;
	D3DXMATRIXA16   m_matViewProj;

	float			m_basePosY; 


	float			m_distance;
	float           m_Fov;
	float           m_TransX;
	float			m_rotX;
	float			m_rotY;
	bool			m_isRButtonDown;

	bool            m_TestDistance;

	POINT			m_ptPrevMouse;

	D3DXVECTOR3*	m_pTarget;



	void CreateViewmatrix();
	


public:
	void Init();
	void Update();
	void SetTarget(D3DXVECTOR3* pTarget) {
		m_pTarget = pTarget; };
	Camera();
	~Camera();

	D3DXVECTOR3 GetCameraUp()
	{
		return m_up;
	}
	void Setdistance(bool _istrue)
	{
		m_TestDistance = _istrue;
	}
	void SetCharacterForword(D3DXVECTOR3* F)
	{
		m_forward = F;
	}
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	float GetRotY()
	{
		return m_rotY;
	}

	D3DXMATRIXA16* GetViewMatrix()
	{
		return &m_matView;
	}
	D3DXMATRIXA16 *GetProjMatrix()
	{
		return& m_matProj;
	}
	D3DXMATRIXA16 *GetViewProjMatrix()
	{
		return &m_matViewProj;
	}


};

