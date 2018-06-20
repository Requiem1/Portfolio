#pragma once

class BDrawingGroup;

class ObjLoader
{
public:
	string m_filePath;
	map<string, MTLTEX*> m_mapMtlTex;

	// surface 계산을 위해 밖으로 빼냄
	vector<VERTEX_PNT> vecPNT;


	ObjLoader();
	~ObjLoader();

	// .Obj 파일을 로드하는 메서드
	// filePath = 경로, fileName = 파일이름, pMat = 월드매트릭스, vecGroup = 실제로 OUT될 버텍스 벡터
	LPD3DXMESH LoadMesh(const char* filePath, const char* fileName, D3DXMATRIXA16 *pMat, OUT vector<MTLTEX*> &vecMtlTex);

	// mtl라이브러리 호출
	void LoadMtlLib(string fullpath);
	void CreateSurface(OUT vector<D3DXVECTOR3>& vecVertex);
	bool CompareStr(char* str1, const char* str2);
};

