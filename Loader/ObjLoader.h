#pragma once

class BDrawingGroup;

class ObjLoader
{
public:
	string m_filePath;
	map<string, MTLTEX*> m_mapMtlTex;

	// surface ����� ���� ������ ����
	vector<VERTEX_PNT> vecPNT;


	ObjLoader();
	~ObjLoader();

	// .Obj ������ �ε��ϴ� �޼���
	// filePath = ���, fileName = �����̸�, pMat = �����Ʈ����, vecGroup = ������ OUT�� ���ؽ� ����
	LPD3DXMESH LoadMesh(const char* filePath, const char* fileName, D3DXMATRIXA16 *pMat, OUT vector<MTLTEX*> &vecMtlTex);

	// mtl���̺귯�� ȣ��
	void LoadMtlLib(string fullpath);
	void CreateSurface(OUT vector<D3DXVECTOR3>& vecVertex);
	bool CompareStr(char* str1, const char* str2);
};

