#pragma once
#include "IScene.h"

class Grid;
class Player;
class DXMap;
class DSkyBox;

class STempLoaderScene : public IScene
{
	Player * m_Pplayer;
	DXMap * m_PdxMap;
	DSkyBox * m_PskyBox;

public:
	STempLoaderScene();
	~STempLoaderScene();

	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

