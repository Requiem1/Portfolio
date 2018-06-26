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
	// 스카이박스
	m_PskyBox = new DSkyBox;
	m_PskyBox->Create("Resource/SkyBox/", "grimmnight");

	// 맵이 먼저 나와서 플레이어의 위치를 잡아줘야한다
	// 다만 지금은 DXMap 파일에 플레이어 위치가 없음..
	// 플레이어매니저에서 위치를 받아줘야할듯
	m_PdxMap = new DXMap;
	m_PdxMap->LoadDXMap("SampleObjMap.DXMap");
	m_PdxMap->Init();
	AddSimpleDisplayObj(m_PdxMap);


	// xfile을 이용한 플레이어 생성
	m_Pplayer = new Player();
	m_Pplayer->SetPosition(&g_PlayerMGR->GetPlayerStartPos(INDEX_PLAYER_1));
	m_Pplayer->SetXfile("Resource/Player/", "player_item.X");
	m_Pplayer->Init();
	AddSimpleDisplayObj(m_Pplayer);
	g_PlayerMGR->AddPlayer(m_Pplayer);


	// 디렉셔널 라이트
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
	g_ZombieMGR->Update();	// 좀비는 매니저에서 따로 업데이트된다
}

void STempLoaderScene::Render()
{
	// 0 키를 누르면 BoundingBox가 Render된다
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
