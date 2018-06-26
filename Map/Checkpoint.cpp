#include "../stdafx.h"
#include "Checkpoint.h"
#include "../Obstacle/CheckpointLever.h"


Checkpoint::Checkpoint()
{
	m_BisLeverOn = false;	// 체크포인트가 동작중인가
	m_BisOccuped = false;	// 점령된 곳인지 확인

	m_siegeTime = 120.1f;	// 점령해야 할 시간 -> 2분!

	// 체크포인트 태그를 넣는다
	g_DisplayObjMGR->AddObjectWithTag(this, CHECKPOINT_TAG);
}


Checkpoint::~Checkpoint()
{
	SAFE_RELEASE(m_Lever);
}

void Checkpoint::Init()
{	
	// 체크포인트의 길이
	m_length = D3DXVECTOR3(20.0f, 10.0f, 20.0f);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixTranslation(&m_matWorld, m_pos.x, m_pos.y, m_pos.z);

	m_BoundingBox->initBoundingBox(NULL, m_length, m_pos);

	// 체크포인트 레버 생성
	m_Lever = new CheckpointLever;
	m_Lever->SetPosition(&m_pos);
	m_Lever->Init();

	m_Lever->SetBisLeverOn(m_BisLeverOn);
}

void Checkpoint::Update()
{
	Debug->AddText("Checkpoint -- 남은 시간 : ");
	Debug->AddText(m_siegeTime);
	Debug->EndLine();


	// 스위치를 켜서 isPointOn이 동작했다면 && 점령된 곳이 아니라면!
	if (m_BisLeverOn == true && m_BisOccuped == false)
		CheckpointTimeCheckFunc();

	if(m_BisLeverOn == false)
		m_Lever->Update();
}

void Checkpoint::Render()
{
	m_BoundingBox->RenderBoundingBox();
	m_Lever->Render();
}

void Checkpoint::CheckpointTimeCheckFunc()
{
	IDisplayObject * Obj = g_DisplayObjMGR->CollideCheckWithTagFunc(this, 1, PLAYER_TAG);

	// 충돌중이다 -> 점령지 위에 있다!!
	if (Obj != NULL)
	{
		m_siegeTime -= g_TimeMGR->GetDeltaTime();

		// 점령 완료
		if (m_siegeTime < D3DX_16F_EPSILON)
			m_BisOccuped = true;
	}
}
