#pragma once

class DMapObstacle;

// 체크포인트 레버
// 누를시 레버가 당기며 작동함
class CheckpointLever : public BaseObject
{
private:
	DMapObstacle *  m_PleverObj;		// 레버 오브젝트!

	CBox			m_LeverSelectBox;	// 레버를 동작시킬수 있는 범위의 바운딩박스
	bool*			m_BisLeverOn;		// 체크포인트가 동작중인가
	D3DXVECTOR3		m_pos;

public:
	CheckpointLever();
	~CheckpointLever();

	void Init();
	void Update();
	void Render();

	void SetBisLeverOn(bool &b) { m_BisLeverOn = &b; }

	D3DXVECTOR3		GetPosition() { return m_pos; }
	void			SetPosition(D3DXVECTOR3* pos) { m_pos = *pos; }
};

