#include "../stdafx.h"
#include "Checkpoint.h"



Checkpoint::Checkpoint()
{

}


Checkpoint::~Checkpoint()
{
}

void Checkpoint::Init()

{	// 체크포인트의 길이
	m_length = D3DXVECTOR3(20.0f, 10.0f, 20.0f);

	m_BisPointOn = true;	// 체크포인트가 동작중인가
	m_BisOccuped = false;	// 점령된 곳인지 확인

	m_siegeTime = 120.1f;	// 점령해야 할 시간 -> 2분!

	// 체크포인트 태그를 넣는다
	g_DisplayObjMGR->AddObjectWithTag(this, CHECKPOINT_TAG);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixTranslation(&m_matWorld, m_pos.x, m_pos.y, m_pos.z);

	m_BoundingBox->initBoundingBox(NULL, m_length, m_pos);
	m_BoundingBox->UpdateBoundingBox(m_matWorld, m_pos);
}

void Checkpoint::Update()
{
	Debug->AddText("Checkpoint -- 남은 시간 : ");
	Debug->AddText(m_siegeTime);
	Debug->EndLine();


	// 스위치를 켜서 isPointOn이 동작했다면 && 점령된 곳이 아니라면!
	if (m_BisPointOn == true && m_BisOccuped == false)
	{
		CheckpointTimeCheckFunc();
	}
}

void Checkpoint::Render()
{
	m_BoundingBox->RenderBoundingBox();
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
		{
			cout << "체크포인트 점령 완료" << endl;
			m_BisOccuped = true;
		}
	}
}
