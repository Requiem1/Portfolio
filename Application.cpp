#include "../Teamproject/stdafx.h"
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
	g_Camera->Init();
}

void Application::Destroy()
{
	g_pTextureMGR->Destroy();
	g_DeviceMGR->Destroy();
}

void Application::Update()
{
	//g_INPUTMGR->Update();
	g_Camera->Update();
}

void Application::Render()
{
	g_Device->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);



	g_Device->EndScene();
	g_Device->Present(NULL, NULL, NULL, NULL);


}

void Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_Camera->WndProc(hWnd, message, wParam, lParam);
}
