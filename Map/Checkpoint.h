#pragma once
#include "../Base/IDisplayObject.h"

class DMapObstacle;

// 체크포인트 클래스
// 정사각형의 체크포인트를 만든 다음
// BoundingBox를 이용하여 플레이어와의 충돌 확인으로 점령중인지를 검사한다
// TimeManager로 특정 시간 이상 그 지점을 점령하고 있다면 점령 완료인걸로 체크된다
class Checkpoint : public IDisplayObject
{
private:
	float			m_length;	// Checkpoint의 length
	DMapObstacle*	m_Lever;	// Checkpoint의 레버. 동작시 카운트를 시작
	
public:
	Checkpoint();
	~Checkpoint();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void CheckpointTimeCheckFunc();

};

