#include "../stdafx.h"
#include "DSkyBox.h"


DSkyBox::DSkyBox()
{
	for (int i = 0; i < MAX_SKY_TEX; i++) 
	{
		m_pTex[i] = NULL;
	}
	
	m_pVB = NULL;

	// ��ī�̹ڽ��� �浹������ �ƴϹǷ� ��
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

	//for (int i = 0; i < MAX_SKY_TEX; i++)
	//{
	//	if (m_pTex[i] != NULL)
	//		SAFE_RELEASE(m_pTex[i]);
	//}
}

void DSkyBox::_LoadTextures(const char* filePath, const char* fileName)
{
	string f = filePath;
	string fullPath = f + "/" + fileName;

	m_pTex[0] = g_TextureMGR->GetTexture(fullPath + "_top.png");
	m_pTex[1] = g_TextureMGR->GetTexture(fullPath + "_front.png");
	m_pTex[2] = g_TextureMGR->GetTexture(fullPath + "_back.png");
	m_pTex[3] = g_TextureMGR->GetTexture(fullPath + "_right.png");
	m_pTex[4] = g_TextureMGR->GetTexture(fullPath + "_left.png");
	m_pTex[5] = g_TextureMGR->GetTexture(fullPath + "_bottom.png");
}

void DSkyBox::_CreateVertex()
{
	if (FAILED(g_Device->CreateVertexBuffer(24 * sizeof(SkyVertex), 0, D3DFVF_SKYVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		_Destroy();
		return;
	}

	SkyVertex vertices[24] =
	{
		{ -450.0f, 450.0f, -450.0f, 0.0f, 0.0f },		// Top vertices
		{ 450.0f, 450.0f, -450.0f, 1.0f, 0.0f },
		{ -450.0f, 450.0f,  450.0f, 0.0f, 1.0f },
		{ 450.0f, 450.0f,  450.0f, 1.0f, 1.0f },

		{ -450.0f,  450.0f, 450.0f, 0.0f, 0.0f },		// Front vertices
		{ 450.0f,  450.0f, 450.0f, 1.0f, 0.0f },
		{ -450.0f, -450.0f, 450.0f, 0.0f, 1.0f },
		{ 450.0f, -450.0f, 450.0f, 1.0f, 1.0f },

		{ 450.0f,  450.0f, -450.0f, 0.0f, 0.0f },		// Back vertices
		{ -450.0f,  450.0f, -450.0f, 1.0f, 0.0f },
		{ 450.0f, -450.0f, -450.0f, 0.0f, 1.0f },
		{ -450.0f, -450.0f, -450.0f, 1.0f, 1.0f },

		{ -450.0f,  450.0f, -450.0f, 0.0f, 0.0f },		// Left vertices
		{ -450.0f,  450.0f,  450.0f, 1.0f, 0.0f },
		{ -450.0f, -450.0f, -450.0f, 0.0f, 1.0f },
		{ -450.0f, -450.0f,  450.0f, 1.0f, 1.0f },

		{ 450.0f,  450.0f,  450.0f, 0.0f, 0.0f },		// Right vertices
		{ 450.0f,  450.0f, -450.0f, 1.0f, 0.0f },
		{ 450.0f, -450.0f,  450.0f, 0.0f, 1.0f },
		{ 450.0f, -450.0f, -450.0f, 1.0f, 1.0f },

		{ -450.0f, -450.0f,  450.0f, 0.0f, 0.0f },		// Bottom vertices
		{ 450.0f, -450.0f,  450.0f, 1.0f, 0.0f },
		{ -450.0f, -450.0f, -450.0f, 0.0f, 1.0f },
		{ 450.0f, -450.0f, -450.0f, 1.0f, 1.0f }
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
	D3DXMatrixIdentity(&m_matWorld);
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
	// D3DCULL_NONE : �޸��� �ø�(Culling)�� ���� �ʴ´�.
	// D3DCULL_CW   : �޸��� ��ȸ������ �ø�(Culling) �Ѵ�.
	// D3DCULL_CCW  : �޸��� ���� ȸ������ �ø�(Culling) �Ѵ�.
	// D3DCULL_FORCE_DWORD : �� �������� ���������� 32 ��Ʈ ����� ������ �Ѵ�. �� ���� ������ �ʾҴ�.
	//g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�ؽ��� ���͸�.(�⺻���� D3DTEXF_POINT)
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// D3DSAMP_ADDRESSU : u ��ǥ�� ����ϴ� �ؽ�ó��巹�̸��. ����Ʈ�� D3DTADDRESS_WRAP �̴�. �� �ڼ��� ������, ��D3DTEXTUREADDRESS ���� �����Ұ�. 
	// D3DSAMP_ADDRESSV : v ��ǥ�� ����ϴ� �ؽ�ó��巹�̸��. ����Ʈ�� D3DTADDRESS_WRAP �̴�. �� �ڼ��� ������, ��D3DTEXTUREADDRESS ���� �����Ұ�. 
	// D3DTADDRESS_CLAMP: ���� [0.0, 1.0] �� ������ �ؽ�ó ��ǥ��, ����, 0.0 ���� 1.0 �� �ؽ�ó �÷��� �����ȴ�. 
	g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_Device->SetRenderState(D3DRS_LIGHTING, false);

	g_Device->SetFVF(D3DFVF_SKYVERTEX);
	g_Device->SetStreamSource(0, m_pVB, 0, sizeof(SkyVertex));


	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);

	for (int i = 0; i < 6; i++)
	{
		g_Device->SetTexture(0, m_pTex[i]);
		g_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);

		g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	}

	g_Device->SetTexture(0, NULL);
	//g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
