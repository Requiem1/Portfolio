#pragma once
#include "../Base/IDisplayObject.h"

// 출저 : http://egloos.zum.com/lsrsp/v/851394
// 이미지 출저 : https://www.quake3world.com/forum/viewtopic.php?t=9242

#define D3DFVF_SKYVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)
#define MAX_SKY_TEX 6

struct SkyVertex
{
	float x, y, z, u, v;
};

class DSkyBox : public IDisplayObject
{
private:
	LPDIRECT3DTEXTURE9			m_pTex[6];		// 텍스처
	LPDIRECT3DVERTEXBUFFER9		m_pVB;			// 버텍스 버퍼

public:
	DSkyBox();
	~DSkyBox();

private:
	void		_Destroy();
	void		_LoadTextures(const char* filePath, const char* fileName);
	void		_CreateVertex();

public:
	void		Create(const char* filePath, const char* fileName);

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};