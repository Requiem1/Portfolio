#include "../stdafx.h"
#include "Checkpoint.h"



Checkpoint::Checkpoint()
{
	m_length = 5.0f;
}


Checkpoint::~Checkpoint()
{
}

void Checkpoint::Init()
{
	// 체크포인트의 길이는 5이다
	m_BoundingBox->initBoundingBox(NULL, m_length);
}

void Checkpoint::Update()
{
	m_BoundingBox->UpdateBoundingBox(m_matWorld, m_pos);
}

void Checkpoint::Render()
{
	m_BoundingBox->RenderBoundingBox();
}
