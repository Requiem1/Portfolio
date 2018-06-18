#pragma once
class Player;
#include "../Base/IAnimationObject.h"
enum FSM_STATE
{
	STATE_MOVE,
	STATE_ATTACK
};
class Zombie : public IAnimationObject
{
private:
	D3DXVECTOR3				m_vPlayerPos;
	D3DXVECTOR3				m_vLookatPlayer;
	D3DXVECTOR3				m_vForward;
	D3DXVECTOR3				m_vUp;
	D3DXVECTOR3				m_vRight;		


	float					m_moveSpeed;
	float					m_rotSpeed;
	Player*					m_pPlayer;
	int						m_state;
	bool					m_bAngleCheck;
	vector<D3DXVECTOR3> vecPos;

public:
	Zombie();
	~Zombie();

	void Init();
	void Update();
	void Render();
	//void SetPlayer();


};
