#pragma once
#include "IScene.h"

class Player;
class DXMap;
class DSkyBox;
class DGameUI;

class STempLoaderScene : public IScene
{
	Player * m_Pplayer;
	DXMap * m_PdxMap;
	DSkyBox * m_PskyBox;
	DGameUI* m_PDGameUI;
public:
	STempLoaderScene();
	~STempLoaderScene();

	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

