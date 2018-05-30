#include "../stdafx.h"
#include "DSkyBox.h"


DSkyBox::DSkyBox()
{
	for (int i = 0; i < MAX_SKY_TEX; i++) 
	{
		m_pTex[i] = NULL;
	}
	
	m_pVB = NULL;

	// 스카이박스도 충돌범위가 아니므로 뺌
	g_DisplayObjMGR->RemoveObject(this);
}


DSkyBox::~DSkyBox()
{
	_Destroy();
}

void DSkyBox::_Destroy()
{
	if (m_pVB != NULL)
	{
		SAFE_RELEASE(m_pVB);
	}

	for (int i = 0; i < MAX_SKY_TEX; i++)
	{
		if (m_pTex[i] != NULL)
			SAFE_RELEASE(m_pTex[i]);
	}
}

void DSkyBox::_LoadTextures(const char* filePath, const char* fileName)
{
	string f = filePath;
	string fullPath = f + "/" + fileName;

	m_pTex[0] = g_pTextureMGR->GetTexture(fullPath + "_top.png");
	m_pTex[1] = g_pTextureMGR->GetTexture(fullPath + "_front.png");
	m_pTex[2] = g_pTextureMGR->GetTexture(fullPath + "_back.png");
	m_pTex[3] = g_pTextureMGR->GetTexture(fullPath + "_right.png");
	m_pTex[4] = g_pTextureMGR->GetTexture(fullPath + "_left.png");
	m_pTex[5] = g_pTextureMGR->GetTexture(fullPath + "_bottom.png");
}

void DSkyBox::_CreateVertex()
{
	if (FAILED(g_pDevice->CreateVertexBuffer(24 * sizeof(SkyVertex), 0, D3DFVF_SKYVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		_Destroy();
		return;
	}

	SkyVertex vertices[24] =
	{
		{ -250.0f, 250.0f, -250.0f, 0.0f, 0.0f },		// Top vertices
		{ 250.0f, 250.0f, -250.0f, 1.0f, 0.0f },
		{ -250.0f, 250.0f,  250.0f, 0.0f, 1.0f },
		{ 250.0f, 250.0f,  250.0f, 1.0f, 1.0f },

		{ -250.0f,  250.0f, 250.0f, 0.0f, 0.0f },		// Front vertices
		{ 250.0f,  250.0f, 250.0f, 1.0f, 0.0f },
		{ -250.0f, -250.0f, 250.0f, 0.0f, 1.0f },
		{ 250.0f, -250.0f, 250.0f, 1.0f, 1.0f },

		{ 250.0f,  250.0f, -250.0f, 0.0f, 0.0f },		// Back vertices
		{ -250.0f,  250.0f, -250.0f, 1.0f, 0.0f },
		{ 250.0f, -250.0f, -250.0f, 0.0f, 1.0f },
		{ -250.0f, -250.0f, -250.0f, 1.0f, 1.0f },

		{ -250.0f,  250.0f, -250.0f, 0.0f, 0.0f },		// Left vertices
		{ -250.0f,  250.0f,  250.0f, 1.0f, 0.0f },
		{ -250.0f, -250.0f, -250.0f, 0.0f, 1.0f },
		{ -250.0f, -250.0f,  250.0f, 1.0f, 1.0f },

		{ 250.0f,  250.0f,  250.0f, 0.0f, 0.0f },		// Right vertices
		{ 250.0f,  250.0f, -250.0f, 1.0f, 0.0f },
		{ 250.0f, -250.0f,  250.0f, 0.0f, 1.0f },
		{ 250.0f, -250.0f, -250.0f, 1.0f, 1.0f },

		{ -250.0f, -250.0f,  250.0f, 0.0f, 0.0f },		// Bottom vertices
		{ 250.0f, -250.0f,  250.0f, 1.0f, 0.0f },
		{ -250.0f, -250.0f, -250.0f, 0.0f, 1.0f },
		{ 250.0f, -250.0f, -250.0f, 1.0f, 1.0f }
	};

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, 24 * sizeof(SkyVertex), (void**)&pVertices, 0)))
	{
		_Destroy();
		return;
	}
	memcpy(pVertices, vertices, 24 * sizeof(SkyVertex));
	m_pVB->Unlock();
}


void DSkyBox::Create(const char* filePath, const char* fileName)
{
	_LoadTextures(filePath, fileName);
	//if (m_pTex[0] == NULL)	_Destroy();

	_CreateVertex();
	//if (m_pVB == NULL)	_Destroy();
}

void DSkyBox::Init()
{
}

void DSkyBox::Update()
{
}

void DSkyBox::Render()
{
	// Setup texture
	// D3DCULL_NONE : 뒷면의 컬링(Culling)는 하지 않는다.
	// D3DCULL_CW   : 뒷면을 우회전으로 컬링(Culling) 한다.
	// D3DCULL_CCW  : 뒷면을 왼쪽 회전으로 컬링(Culling) 한다.
	// D3DCULL_FORCE_DWORD : 이 열거형을 강제적으로 32 비트 사이즈에 컴파일 한다. 이 값은 사용되지 않았다.
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//텍스쳐 필터링.(기본값은 D3DTEXF_POINT)
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// D3DSAMP_ADDRESSU : u 좌표로 사용하는 텍스처어드레싱모드. 디폴트는 D3DTADDRESS_WRAP 이다. 더 자세한 정보는, 「D3DTEXTUREADDRESS 」를 참조할것. 
	// D3DSAMP_ADDRESSV : v 좌표로 사용하는 텍스처어드레싱모드. 디폴트는 D3DTADDRESS_WRAP 이다. 더 자세한 정보는, 「D3DTEXTUREADDRESS 」를 참조할것. 
	// D3DTADDRESS_CLAMP: 범위 [0.0, 1.0] 의 외측의 텍스처 좌표가, 각각, 0.0 으로 1.0 의 텍스처 컬러로 설정된다. 
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	g_pDevice->SetFVF(D3DFVF_SKYVERTEX);
	g_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(SkyVertex));

	for (int i = 0; i < 6; i++)
	{
		g_pDevice->SetTexture(0, m_pTex[i]);
		g_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);

		g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	}

	g_pDevice->SetTexture(0, NULL);
	//g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
