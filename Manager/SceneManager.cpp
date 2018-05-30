#include "../stdafx.h"
#include "SceneManager.h"
#include "../Scene/IScene.h"
SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	Destroy();
}

void SceneManager::Init()
{
	/*
	
	m_vecScene.push_back(new IntroScene());
	m_vecScene.push_back(new SceneHeightMap());

	//SetCurrentScene(SCENE_INTRO);
	SetCurrentScene(SCENE_HEIGHTMAP);
	*/
}

void SceneManager::Destroy()
{
	for(int i = 0; i < m_vecScene.size(); i++)
		SAFE_RELEASE(m_vecScene[i]);
}

void SceneManager::Update()
{
	SAFE_UPDATE(m_pCurrScene);
}

void SceneManager::Render()
{
	SAFE_RENDER(m_pCurrScene);
}

void SceneManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCurrScene)
		m_pCurrScene->WndProc(hWnd, message, wParam, lParam);
}

void SceneManager::SetCurrentScene(size_t index)
{
	if (index >= m_vecScene.size()) return;

	IScene* pScene = m_vecScene[index];
	if (pScene)
	{
		pScene->Init();
		m_pCurrScene = pScene;
	}
}
