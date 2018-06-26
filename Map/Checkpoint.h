#pragma once
#include "../Base/IDisplayObject.h"

class DMapObstacle;
class CheckpointLever;

// üũ����Ʈ Ŭ����
// ���簢���� üũ����Ʈ�� ���� ����
// BoundingBox�� �̿��Ͽ� �÷��̾���� �浹 Ȯ������ ������������ �˻��Ѵ�
// TimeManager�� Ư�� �ð� �̻� �� ������ �����ϰ� �ִٸ� ���� �Ϸ��ΰɷ� üũ�ȴ�
class Checkpoint : public IDisplayObject
{
private:
	D3DXVECTOR3			m_length;		// Checkpoint�� length -> xyz���� ������ box�� ����
	CheckpointLever*	m_Lever;		// Checkpoint�� ����. ���۽� ī��Ʈ�� ����

	bool				m_BisLeverOn;	// üũ����Ʈ�� �������ΰ�
	bool				m_BisOccuped;	// ���ɵ� ������ Ȯ��

	float				m_siegeTime;	// �����ؾ��� �ð�!

	size_t				m_cpIndex;		// üũ����Ʈ �ε���
	
public:
	Checkpoint();
	~Checkpoint();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void CheckpointTimeCheckFunc();

};

