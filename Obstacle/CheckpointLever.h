#pragma once

class DMapObstacle;

// üũ����Ʈ ����
// ������ ������ ���� �۵���
class CheckpointLever : public BaseObject
{
private:
	DMapObstacle *  m_PleverObj;		// ���� ������Ʈ!

	CBox			m_LeverSelectBox;	// ������ ���۽�ų�� �ִ� ������ �ٿ���ڽ�
	bool*			m_BisLeverOn;		// üũ����Ʈ�� �������ΰ�
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

