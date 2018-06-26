#include "stdafx.h"
#include "Application.h"



Application::Application()
{
}


Application::~Application()
{
}

void Application::Init()
{
	g_DeviceMGR->Init();
	g_INPUTMGR->Init();	
	g_SoundMGR->Init();		// 사운드 init
	g_SceneMGR->Init();		// 씬 init
}

void Application::Destroy()
{
	g_SceneMGR->Destroy();
	g_DisplayObjMGR->Destroy();
	g_TextureMGR->Destroy();
	Debug->Destroy();
	g_DeviceMGR->Destroy();	// Device 폭파는 가장 끝에
}

void Application::Update()
{
	//g_INPUTMGR->Update();
	g_TimeMGR->Update();	// Time 업데이트
	g_INPUTMGR->Update();	
	g_SoundMGR->Update();	// 사운드 update

	g_SceneMGR->Update();	// 씬 update는 가장 끝에
}

void Application::Render()
{
	g_Device->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);

	g_Device->BeginScene();

	g_SceneMGR->Render();
	
	Debug->Print();
	Debug->InitText();

	g_Device->EndScene();
	g_Device->Present(NULL, NULL, NULL, NULL);
}

void Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_SceneMGR->WndProc(hWnd, message, wParam, lParam);
}
