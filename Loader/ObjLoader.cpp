#include "../stdafx.h"
#include "../Loader/ObjLoader.h"
#include "../Utility/BDrawingGroup.h"

ObjLoader::ObjLoader()
{
}


ObjLoader::~ObjLoader()
{
}

//loader.LoadMesh("resources/obj", "Map.obj", &matWorld, m_vecMtlTex);	// 메쉬 로드
LPD3DXMESH ObjLoader::LoadMesh(const char* filePath, const char* fileName, D3DXMATRIXA16* pMat, OUT vector<MTLTEX*>& vecMtlTex)
{
	vector<D3DXVECTOR3> vecP;
	vector<D3DXVECTOR2> vecT;
	vector<D3DXVECTOR3> vecN;
	vector<DWORD> vecAttBuf; //속성정보
	string mtlName;

	char szToken[128];
	std::ifstream fin;

	m_filePath = filePath;
	string fullPath = m_filePath + "/" + fileName;

	fin.open(fullPath);

	if (fin.is_open() == false)
		return NULL;

	while (fin.eof() == false)
	{
		fin >> szToken;

		if (CompareStr(szToken, "mtllib"))
		{
			string fileName;
			fin >> fileName;

			if(!m_filePath.empty())
				fileName = m_filePath + "/" + fileName;

			LoadMtlLib(fileName.c_str());

			// 로드 후 MtlTex의 ID를 SubSet의 ID와 맞춰줌
			vecMtlTex.resize(m_mapMtlTex.size());
			for (auto p : m_mapMtlTex)
				vecMtlTex[p.second->id] = p.second; //MtlTex id에 인덱스에 맞게 넣어준다 0번 서브셋을 사용할땐 0번 mtltex를 사용한다

		}
		else if (CompareStr(szToken, "v"))
		{
			float x, y, z;
			fin.getline(szToken, 128);
			sscanf_s(szToken, "%f %f %f", &x, &y, &z);
			vecP.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (CompareStr(szToken, "vt"))
		{
			float x, y;
			fin.getline(szToken, 128);
			sscanf_s(szToken, "%f %f *%f", &x, &y);
			vecT.push_back(D3DXVECTOR2(x, y));
		}
		else if (CompareStr(szToken, "vn"))
		{
			float x, y, z;
			fin.getline(szToken, 128);
			sscanf_s(szToken, "%f %f %f", &x, &y, &z);
			vecN.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (CompareStr(szToken, "usemtl"))
		{
			fin >> szToken;
			mtlName = szToken;
		}
		else if (CompareStr(szToken, "f"))
		{
			// 3개 또는 4개의 정점을 받는 인덱스
			int aIndex[4][3];
			fin.getline(szToken, 128);

			char* context = NULL;
			char* ptr = strtok_s(szToken, " ", &context);
			int idxNum = 0;

			while (ptr)
			{
				sscanf_s(ptr, "%d/%d/%d",
					&aIndex[idxNum][0], &aIndex[idxNum][1], &aIndex[idxNum][2]);

				idxNum++;
				ptr = strtok_s(NULL, " ", &context);
			}

			// 정점 3개를 받음
			if (idxNum == 3)
			{
				for (int i = 0; i < 3; i++)
				{
					VERTEX_PNT pnt;
					pnt.p = vecP[aIndex[i][0] - 1];
					pnt.t = vecT[aIndex[i][1] - 1];
					pnt.n = vecN[aIndex[i][2] - 1];

					if (pMat)
					{
						D3DXVec3TransformCoord(&pnt.p, &pnt.p, pMat);
						D3DXVec3TransformNormal(&pnt.n, &pnt.n, pMat);
					}
					vecPNT.push_back(pnt);
				}

				vecAttBuf.push_back(m_mapMtlTex[mtlName]->id); //면하나에 속성 id 하나
			}
			// 정점 4개를 받음
			else if (idxNum == 4)
			{
				// 0 1 2
				for (int i = 0; i < 3; i++)
				{
					VERTEX_PNT pnt;
					pnt.p = vecP[aIndex[i][0] - 1];
					pnt.t = vecT[aIndex[i][1] - 1];
					pnt.n = vecN[aIndex[i][2] - 1];

					if (pMat)
					{
						D3DXVec3TransformCoord(&pnt.p, &pnt.p, pMat);
						D3DXVec3TransformNormal(&pnt.n, &pnt.n, pMat);
					}
					vecPNT.push_back(pnt);
				}

				vecAttBuf.push_back(m_mapMtlTex[mtlName]->id); //면하나에 속성 id 하나


				// 0 2 3
				VERTEX_PNT pnt0;
				pnt0.p = vecP[aIndex[0][0] - 1];
				pnt0.t = vecT[aIndex[0][1] - 1];
				pnt0.n = vecN[aIndex[0][2] - 1];

				if (pMat)
				{
					D3DXVec3TransformCoord(&pnt0.p, &pnt0.p, pMat);
					D3DXVec3TransformNormal(&pnt0.n, &pnt0.n, pMat);
				}
				vecPNT.push_back(pnt0);

				for (int i = 2; i <= 3; i++)
				{
					VERTEX_PNT pnt;
					pnt.p = vecP[aIndex[i][0] - 1];
					pnt.t = vecT[aIndex[i][1] - 1];
					pnt.n = vecN[aIndex[i][2] - 1];

					if (pMat)
					{
						D3DXVec3TransformCoord(&pnt.p, &pnt.p, pMat);
						D3DXVec3TransformNormal(&pnt.n, &pnt.n, pMat);
					}
					vecPNT.push_back(pnt);
				}

				vecAttBuf.push_back(m_mapMtlTex[mtlName]->id); //면하나에 속성 id 하나
			}


		}
	}

	m_mapMtlTex.clear();
	fin.close();

	/* 매쉬생성 및 정보 채우기, 최적화 */
	LPD3DXMESH pMesh = NULL;

	// 면수, 버택스 수, 나머지는 다른것들과 같음!
	// WORD의 최대 크기가 65536(16비트)이므로 넘어가면 32비트로 변환해줌
	//if(vecPNT.size() >= 65536)
		D3DXCreateMeshFVF(vecPNT.size() / 3, vecPNT.size(), D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_PNT::FVF, g_Device, &pMesh);
	//else
	//	D3DXCreateMeshFVF(vecPNT.size() / 3, vecPNT.size(), D3DXMESH_MANAGED, VERTEX_PNT::FVF, g_Device, &pMesh);

	//1. 버퍼 3개 만들자!!!
	// 버텍스 버퍼
	VERTEX_PNT* pV = NULL;
	DWORD flags = 0;
	pMesh->LockVertexBuffer(flags, (LPVOID*)&pV);
	memcpy(pV, &vecPNT[0], vecPNT.size() * sizeof(VERTEX_PNT));
	pMesh->UnlockVertexBuffer();

	// 인덱스 버퍼
	DWORD* pI = NULL;
	pMesh->LockIndexBuffer(flags, (LPVOID*)&pI);
	for (size_t i = 0; i < vecPNT.size(); ++i)
		pI[i] = static_cast<DWORD>(i);
	pMesh->UnlockIndexBuffer();

	// 애트리뷰트 버퍼
	DWORD* pA = NULL;
	pMesh->LockAttributeBuffer(flags, &pA);
	memcpy(pA, &vecAttBuf[0], vecAttBuf.size() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();

	// 2. 인접정보 생성
	vector<DWORD> vecAdjacency(pMesh->GetNumFaces() * 3);
	pMesh->GenerateAdjacency(FLT_EPSILON, &vecAdjacency[0]);

	//DWORD* aAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	//pMesh->GenerateAdjacency(1e-6f, aAdjacency);
	//pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL);

	// 3. 최적화
	// 메쉬의 면 및 정점의 차례 변경을 제어해, 퍼포먼스를 최적화한다.
	pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, &vecAdjacency[0], 0, 0, 0);
	//SAFE_DELETE_ARRAY(aAdjacency);

	return pMesh;
}

void ObjLoader::LoadMtlLib(string fullPath)
{
	string mtlName;
	DWORD mtltexID = 0;		// ID 계산용

	char szToken[128];
	std::ifstream fin;

	fin.open(fullPath);

	if (fin.is_open() == false)
		return;

	while (fin.eof() == false)
	{
		fin >> szToken;

		if (CompareStr(szToken, "newmtl"))
		{
			fin >> mtlName;

			SAFE_RELEASE(m_mapMtlTex[mtlName]);
			m_mapMtlTex[mtlName] = new MTLTEX;
			m_mapMtlTex[mtlName]->id = mtltexID++;	// MTLTEX 생성시마다 다른 아이디를 가지도록 ++을 해주자
		}
		else if (CompareStr(szToken, "Ka"))
		{
			D3DCOLORVALUE c;
			fin.getline(szToken, 128);
			sscanf_s(szToken, "%f %f %f", &c.r, &c.g, &c.b);
			c.a = 1.0f;
			m_mapMtlTex[mtlName]->material.Ambient = c;
		}
		else if (CompareStr(szToken, "Kd"))
		{
			D3DCOLORVALUE c;
			fin.getline(szToken, 128);
			sscanf_s(szToken, "%f %f %f", &c.r, &c.g, &c.b);
			c.a = 1.0f;
			m_mapMtlTex[mtlName]->material.Diffuse = c;
		}
		else if (CompareStr(szToken, "Ks"))
		{
			D3DCOLORVALUE c;
			fin.getline(szToken, 128);
			sscanf_s(szToken, "%f %f %f", &c.r, &c.g, &c.b);
			c.a = 1.0f;
			m_mapMtlTex[mtlName]->material.Specular = c;
		}
		else if (CompareStr(szToken, "map_Kd"))
		{
			string filename;
			fin >> filename;
			filename = m_filePath + "/" + filename;

			m_mapMtlTex[mtlName]->SetTexture(
				g_TextureMGR->GetTexture(filename));
		}
	}

	fin.close();
}


 // 바닥을 생성해준다
void ObjLoader::CreateSurface(OUT vector<D3DXVECTOR3>& vecVertex)
{
	int PNTsize = vecPNT.size();
	vecVertex.reserve(PNTsize / 3);

	for (size_t i = 0; i < PNTsize; i += 3)
	{
		D3DXVECTOR3 normal;
		DXUtil::ComputeNormal(&normal, &vecPNT[i].p, &vecPNT[i + 1].p, &vecPNT[i + 2].p);

		// 내적 결과가 cos 45도 이하인 경우 발판
		if ( D3DXVec3Dot(&normal, &D3DXVECTOR3(0, 1, 0)) > cos(D3DX_PI / 4) )
		{
			vecVertex.push_back(vecPNT[i + 0].p);
			vecVertex.push_back(vecPNT[i + 1].p);
			vecVertex.push_back(vecPNT[i + 2].p);
		}
	}
}

bool ObjLoader::CompareStr(char * str1, const char * str2)
{
	return strcmp(str1, str2) == 0;
}
