#include "../stdafx.h"
#include "STempLoaderScene.h"

#include "../Obstacle/DMapObstacle.h"

#include "../Obstacle/Grid.h"
#include "../Player/Player.h"
#include "../Map/DXMap.h"
#include "../Map/DSkyBox.h"

STempLoaderScene::STempLoaderScene()
{
}


STempLoaderScene::~STempLoaderScene()
{
	g_ZombieMGR->Destory();

	OnDestructIScene();
	SAFE_RELEASE(m_PskyBox);
}

void STempLoaderScene::Init()
{
	// ��ī�̹ڽ�
	m_PskyBox = new DSkyBox;
	m_PskyBox->Create("Resource/SkyBox/", "grimmnight");

	// ���� ���� ���ͼ� �÷��̾��� ��ġ�� �������Ѵ�
	// �ٸ� ������ DXMap ���Ͽ� �÷��̾� ��ġ�� ����..
	// �÷��̾�Ŵ������� ��ġ�� �޾�����ҵ�
	m_PdxMap = new DXMap;
	m_PdxMap->LoadDXMap("SampleObjMap.DXMap");
	m_PdxMap->Init();
	AddSimpleDisplayObj(m_PdxMap);


	// xfile�� �̿��� �÷��̾� ����
	m_Pplayer = new Player();
	m_Pplayer->SetPosition(&g_PlayerMGR->GetPlayerStartPos(INDEX_PLAYER_1));
	m_Pplayer->SetXfile("Resource/Player/", "player_item.X");
	m_Pplayer->Init();
	AddSimpleDisplayObj(m_Pplayer);
	g_PlayerMGR->AddPlayer(m_Pplayer);


	// �𷺼ų� ����Ʈ
	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&dir, &dir);
	D3DLIGHT9 light = DXUtil::InitDirectional(&dir, &WHITE);
	g_Device->SetLight(0, &light);
	g_Device->LightEnable(0, true);


	g_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_Device->SetRenderState(D3DRS_LIGHTING, true);


	g_ZombieMGR->AddZombie(3);
	g_ZombieMGR->init();
}

void STempLoaderScene::Update()
{
	OnUpdateIScene();
	g_ZombieMGR->Update();	// ����� �Ŵ������� ���� ������Ʈ�ȴ�
}

void STempLoaderScene::Render()
{
	// 0 Ű�� ������ BoundingBox�� Render�ȴ�
	if (GetAsyncKeyState('0') & 0x0001)
		g_DisplayObjMGR->SetBoundingBoxRender();

	SAFE_RENDER(m_PskyBox);

	OnRenderIScene();
	g_ZombieMGR->Render();
}

void STempLoaderScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_Pplayer->WndProc(hWnd, message, wParam, lParam);
}
