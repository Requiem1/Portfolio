#include "../stdafx.h"
#include "Checkpoint.h"
#include "../Obstacle/CheckpointLever.h"


Checkpoint::Checkpoint()
{
	m_BisLeverOn = false;	// üũ����Ʈ�� �������ΰ�
	m_BisOccuped = false;	// ���ɵ� ������ Ȯ��

	m_siegeTime = 120.1f;	// �����ؾ� �� �ð� -> 2��!

	// üũ����Ʈ �±׸� �ִ´�
	g_DisplayObjMGR->AddObjectWithTag(this, CHECKPOINT_TAG);
}


Checkpoint::~Checkpoint()
{
	SAFE_RELEASE(m_Lever);
}

void Checkpoint::Init()
{	
	// üũ����Ʈ�� ����
	m_length = D3DXVECTOR3(20.0f, 10.0f, 20.0f);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixTranslation(&m_matWorld, m_pos.x, m_pos.y, m_pos.z);

	m_BoundingBox->initBoundingBox(NULL, m_length, m_pos);

	// üũ����Ʈ ���� ����
	m_Lever = new CheckpointLever;
	m_Lever->SetPosition(&m_pos);
	m_Lever->Init();

	m_Lever->SetBisLeverOn(m_BisLeverOn);
}

void Checkpoint::Update()
{
	Debug->AddText("Checkpoint -- ���� �ð� : ");
	Debug->AddText(m_siegeTime);
	Debug->EndLine();


	// ����ġ�� �Ѽ� isPointOn�� �����ߴٸ� && ���ɵ� ���� �ƴ϶��!
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

	// �浹���̴� -> ������ ���� �ִ�!!
	if (Obj != NULL)
	{
		m_siegeTime -= g_TimeMGR->GetDeltaTime();

		// ���� �Ϸ�
		if (m_siegeTime < D3DX_16F_EPSILON)
			m_BisOccuped = true;
	}
}
