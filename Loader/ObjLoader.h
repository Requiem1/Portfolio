#pragma once

class BDrawingGroup;

class ObjLoader
{
public:
	string m_filePath;
	map<string, MTLTEX*> m_mapMtlTex;

	ObjLoader();
	~ObjLoader();

	// OUT -> Load된 값을 거기에 넣어주겠다
	void Load(const char* filePath, const char* fileName, D3DXMATRIXA16 *pMat, OUT vector<BDrawingGroup*> &vecGroup);
	void LoadSurface(const char* fullpath, D3DXMATRIXA16* pMat, OUT vector<D3DXVECTOR3> &vecVertex);
	LPD3DXMESH LoadMesh(const char* filePath, const char* fileName, D3DXMATRIXA16 *pMat, OUT vector<MTLTEX*> &vecMtlTex);

	void LoadMtlLib(string fullpath);
	bool CompareStr(char* str1, const char* str2);
};

