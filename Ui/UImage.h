#pragma once
#include "DUIObject.h"

class UImage :public DUIObject
{
private:
	LPDIRECT3DTEXTURE9 m_Ptex;
public:
	UImage(LPD3DXSPRITE Psprite, int uiTag = -1);
	virtual	~UImage();

	virtual void Render()override;

	void SetTexture(string fullPath);

};

