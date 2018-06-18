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
	// 이거 DXMap에서 체크포인트 열때 해야됨!
	m_PleverObj = new DMapObstacle();
	m_PleverObj->SetObstacle("Lever.obj", m_pos);

	// 장애물 태그 추가
	g_DisplayObjMGR->AddObjectWithTag(m_PleverObj, NORMAL_OBSTACLE_TAG);

	m_LeverSelectBox.initBoundingBox(NULL, D3DXVECTOR3(4.0f, 4.0, 4.0f), m_pos);
}

void CheckpointLever::Update()
{
	IDisplayObject * Obj = g_DisplayObjMGR->CollideCheckWithTagFunc(m_LeverSelectBox, 1, PLAYER_TAG);
	if (Obj != NULL)
	{
		Debug->AddText("CheckpointLever 충돌");
		Debug->EndLine();

		// F 누를시 스위치 작동
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
