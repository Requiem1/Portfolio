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
	g_SoundMGR->Init();		// ���� init
	g_SceneMGR->Init();		// �� init
}

void Application::Destroy()
{
	g_SceneMGR->Destroy();
	g_DisplayObjMGR->Destroy();
	g_TextureMGR->Destroy();
	Debug->Destroy();
	g_DeviceMGR->Destroy();	// Device ���Ĵ� ���� ����
}

void Application::Update()
{
	//g_INPUTMGR->Update();
	g_TimeMGR->Update();	// Time ������Ʈ
	g_INPUTMGR->Update();	
	g_SoundMGR->Update();	// ���� update

	g_SceneMGR->Update();	// �� update�� ���� ����
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
