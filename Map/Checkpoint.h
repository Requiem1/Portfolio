#pragma once
#include "../Base/IDisplayObject.h"

class DMapObstacle;

// üũ����Ʈ Ŭ����
// ���簢���� üũ����Ʈ�� ���� ����
// BoundingBox�� �̿��Ͽ� �÷��̾���� �浹 Ȯ������ ������������ �˻��Ѵ�
// TimeManager�� Ư�� �ð� �̻� �� ������ �����ϰ� �ִٸ� ���� �Ϸ��ΰɷ� üũ�ȴ�
class Checkpoint : public IDisplayObject
{
private:
	float			m_length;	// Checkpoint�� length
	DMapObstacle*	m_Lever;	// Checkpoint�� ����. ���۽� ī��Ʈ�� ����
	
public:
	Checkpoint();
	~Checkpoint();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void CheckpointTimeCheckFunc();

};

