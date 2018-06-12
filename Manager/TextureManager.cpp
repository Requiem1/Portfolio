#include "../stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(char * fullPath)
{
	if (fullPath == NULL) return NULL;

	return GetTexture(string(fullPath));
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(string fullPath)
{
	if (fullPath == "") return NULL;

	if (m_mapTexture.find(fullPath) == m_mapTexture.end())
	{
		D3DXCreateTextureFromFileA(g_Device, fullPath.c_str(),
			&m_mapTexture[fullPath]);
	}
	return m_mapTexture[fullPath];
}

LPDIRECT3DTEXTURE9 TextureManager::GetTextureEx(string fullPath)
{
	if (fullPath == "") return NULL;

	if (m_mapTexture.find(fullPath) == m_mapTexture.end())
	{
		CString _str = fullPath.c_str();

		D3DXCreateTextureFromFileEx(g_Device, _str, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), NULL, NULL, &m_mapTexture[fullPath]);
	}
	return m_mapTexture[fullPath];
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(const wchar_t* fullPath)
{
	CT2CA pszConvertedAnsiString(fullPath);
	std::string _str(pszConvertedAnsiString);
	return GetTexture(_str);
}


void TextureManager::Destroy()
{
	for (auto it : m_mapTexture)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapTexture.clear();
}
