#pragma once

#define g_TextureMGR TextureManager::GetInstance()

class TextureManager
{
	SINGLETON(TextureManager)
private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;

public:
	LPDIRECT3DTEXTURE9 GetTexture(char* fullPath);
	LPDIRECT3DTEXTURE9 GetTexture(string fullPath);
	LPDIRECT3DTEXTURE9 GetTextureEx(string fullPath);
	LPDIRECT3DTEXTURE9 GetTexture(const wchar_t* fullPath);

	void Destroy();
};

