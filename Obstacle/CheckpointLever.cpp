#include "../stdafx.h"
#include "CheckpointLever.h"
#include "../Base/BaseObject.h"
#include "../Obstacle/DMapObstacle.h"



CheckpointLever::CheckpointLever()
{
}


CheckpointLever::~CheckpointLever()
{
	SAFE_RELEASE(m_PleverObj);
}

void CheckpointLever::Init()
{
	// �̰� DXMap���� üũ����Ʈ ���� �ؾߵ�!
	m_PleverObj = new DMapObstacle();
	m_PleverObj->SetObstacle("Lever.obj", m_pos);

	// ��ֹ� �±� �߰�
	g_DisplayObjMGR->AddObjectWithTag(m_PleverObj, NORMAL_OBSTACLE_TAG);

	m_LeverSelectBox.initBoundingBox(NULL, D3DXVECTOR3(4.0f, 4.0, 4.0f), m_pos);
}

void CheckpointLever::Update()
{
	IDisplayObject * Obj = g_DisplayObjMGR->CollideCheckWithTagFunc(m_LeverSelectBox, 1, PLAYER_TAG);
	if (Obj != NULL)
	{
		Debug->AddText("CheckpointLever �浹");
		Debug->EndLine();

		// F ������ ����ġ �۵�
		if (GetAsyncKeyState('F') & 0x8000)
		{
			*m_BisLeverOn = true;
		}
	}
}

void CheckpointLever::Render()
{
	m_LeverSelectBox.RenderBoundingBox();
	m_PleverObj->Render();
}
