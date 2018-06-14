#include "../stdafx.h"
#include "Checkpoint.h"



Checkpoint::Checkpoint()
{

}


Checkpoint::~Checkpoint()
{
}

void Checkpoint::Init()

{	// üũ����Ʈ�� ����
	m_length = D3DXVECTOR3(20.0f, 10.0f, 20.0f);

	m_BisPointOn = true;	// üũ����Ʈ�� �������ΰ�
	m_BisOccuped = false;	// ���ɵ� ������ Ȯ��

	m_siegeTime = 120.1f;	// �����ؾ� �� �ð� -> 2��!

	// üũ����Ʈ �±׸� �ִ´�
	g_DisplayObjMGR->AddObjectWithTag(this, CHECKPOINT_TAG);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixTranslation(&m_matWorld, m_pos.x, m_pos.y, m_pos.z);

	m_BoundingBox->initBoundingBox(NULL, m_length, m_pos);
	m_BoundingBox->UpdateBoundingBox(m_matWorld, m_pos);
}

void Checkpoint::Update()
{
	Debug->AddText("Checkpoint -- ���� �ð� : ");
	Debug->AddText(m_siegeTime);
	Debug->EndLine();


	// ����ġ�� �Ѽ� isPointOn�� �����ߴٸ� && ���ɵ� ���� �ƴ϶��!
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

	// �浹���̴� -> ������ ���� �ִ�!!
	if (Obj != NULL)
	{
		m_siegeTime -= g_TimeMGR->GetDeltaTime();

		// ���� �Ϸ�
		if (m_siegeTime < D3DX_16F_EPSILON)
		{
			cout << "üũ����Ʈ ���� �Ϸ�" << endl;
			m_BisOccuped = true;
		}
	}
}
