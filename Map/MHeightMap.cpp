#include "../stdafx.h"
#include "MHeightMap.h"
//#include "Ray.h"

MHeightMap::MHeightMap()
{
	m_pMesh = NULL;
	m_pMtlTex = NULL;

	// 맵이므로 오브젝트에서 뺀다
	g_DisplayObjMGR->RemoveObject(this);
}


MHeightMap::~MHeightMap()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMtlTex);
}

void MHeightMap::Load(const char * fullPath, D3DXMATRIXA16 * pMat)
{
	vector<VERTEX_PNT> vecVertex;
	vecVertex.resize(m_dimension * m_dimension);
	m_vecVertex.resize(m_dimension * m_dimension);

	// raw 파일을 불러온다
	std::ifstream fin(fullPath, std::ios::binary);

	// raw파일에는 각 위치의 높이(y)만이 들어있다!
	// 2차원 배열의 1차원 위치 형태(x축은 1씩, x축 한줄 다 끝나면 z축 1올라감)이므로
	// z( x( input ) ) 형태로 입력받는다
	for (int z = 0; z < m_dimension; z++)
	{
		for (int x = 0; x < m_dimension; x++)
		{
			int index = z * m_dimension + x;
			int y = fin.get();
			
			vecVertex[index].p = D3DXVECTOR3(x, y, z);
			vecVertex[index].n = D3DXVECTOR3(0, 1, 0);
			vecVertex[index].t = D3DXVECTOR2( x / (float)m_numTile,  1 - (z / (float)m_numTile) );	
			// ㄴ전체 텍스쳐가 0,0부터 m_numTile, m_numTile 까지라고 치면
			// ㄴ현재 텍스쳐의 위치는 x와 z의 움직인 거리값이므로
			// ㄴ x / m_numTile, 1 - z / m_numTile 이다!

			// pMat이 있다면 그만큼 이동해준다!
			if (pMat != NULL)
			{
				D3DXVec3TransformCoord(&vecVertex[index].p,	&vecVertex[index].p, pMat);
			}

			// 캐릭터가 지형에서 움직일 수 있도록 높이를 계산하기 위해 받는다
			m_vecVertex[index] = vecVertex[index].p;
		}
	}

	fin.close();

	// 크기는 pMat으로!
	if (pMat != NULL)
	{
		m_scale.x = pMat->_11;	// 현재 위치 x
		m_scale.z = pMat->_33;	// 현재 위치 z
	}

	m_size.x = m_numTile * m_scale.x;
	m_size.z = m_numTile * m_scale.z;

	// 각 타일(2개의 삼각형) 제작
	// 인덱스 버퍼로!
	vector<DWORD> vecIndex;
	vecIndex.reserve(m_numTile * m_numTile * 3 * 2);

	// set index
	for (int z = 0; z < m_numTile; z++)
	{
		for (int x = 0; x < m_numTile; x++)
		{
			// 1--3
			// 0--2
			int _0 = (z + 0) * m_dimension + x + 0;
			int _1 = (z + 1) * m_dimension + x + 0;
			int _2 = (z + 0) * m_dimension + x + 1;
			int _3 = (z + 1) * m_dimension + x + 1;

			// 각 타일에 넣어줄 인덱스 push-back
			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);

			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
			vecIndex.push_back(_3);
		}
	}


	// set normal
	for (int z = 1; z < m_numTile; z++)
	{
		for (int x = 1; x < m_numTile; x++)
		{
			//--u--
			//l-n-r
			//--d--
			int index = z * m_dimension + x;

			int l = index - 1;
			int r = index + 1;
			int u = index + m_dimension;
			int d = index - m_dimension;

			// n의 노말값은 up-down left-right 로 이루어진 면의 외적 벡터
			D3DXVECTOR3 du = m_vecVertex[u] - m_vecVertex[d];
			D3DXVECTOR3 lr = m_vecVertex[r] - m_vecVertex[l];
			D3DXVECTOR3 n;

			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);

			vecVertex[index].n = n;
		}
	}

	// 인덱스가 16비트가 아니라 32비트이다 (범위가 257 * 257 -> 2바이트 이상이므로)
	// D3DXMESH_32BIT를 붙이면 된다
	D3DXCreateMeshFVF(vecIndex.size() / 3, vecVertex.size(), 
		D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_PNT::FVF,	g_Device, &m_pMesh);

	// 버텍스 버퍼
	VERTEX_PNT* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(VERTEX_PNT));
	m_pMesh->UnlockVertexBuffer();

	// 인덱스 버퍼
	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));	// 삼각형 당 하나니까 size / 3
	m_pMesh->UnlockIndexBuffer();

	// 애트리뷰트
	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	// 인접정보 생성
	// 삼각형 면 3개 -> 인접 3개이므로 곱하기 3
	vector<DWORD> vecAdjBuffer(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(FLT_EPSILON, &vecAdjBuffer[0]);

	// 메쉬의 면 및 정점의 차례 변경을 제어해, 퍼포먼스를 최적화한다.
	// D3DXMESHOPT_ATTRSORT -> 애트리뷰트가 하나이므로 정렬 필요 X
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, &vecAdjBuffer[0], 0, 0, 0);
}

void MHeightMap::SetMtlTex(D3DMATERIAL9 & mtl, LPDIRECT3DTEXTURE9 pTex)
{
	m_pMtlTex = new MTLTEX;
	m_pMtlTex->SetMaterial(mtl);
	m_pMtlTex->SetTexture(pTex);
}

void MHeightMap::Init()
{
	SetSurface();
}

void MHeightMap::Update()
{
}

void MHeightMap::Render()
{
	g_Device->SetRenderState(D3DRS_LIGHTING, true);
	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_Device->SetMaterial(&m_pMtlTex->GetMaterial());
	g_Device->SetTexture(0, m_pMtlTex->GetTexture());
	m_pMesh->DrawSubset(0);
	g_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecObstacleVertex.size() / 3, &m_vecObstacleVertex[0], sizeof(D3DXVECTOR3));
	//g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_Device->SetRenderState(D3DRS_FOGENABLE, false);
}

bool MHeightMap::GetHeight(OUT float & height, const D3DXVECTOR3 & pos)
{
	if (pos.x < 0 || pos.z < 0 || pos.x > m_size.x || pos.z > m_size.z)
	{
		height = 0;
		return false;
	}

	// 소수점을 뺀 현재 위치
	int nX = (int)(pos.x / m_scale.x);
	int nZ = (int)(pos.z / m_scale.z);

	// 현재 칸에서 이동한 위치량
	float deltaX = (pos.x / m_scale.x) - nX;
	float deltaZ = (pos.z / m_scale.z) - nZ;

	int index0 = (nZ + 0) * m_dimension + (nX + 0);
	int index1 = (nZ + 1) * m_dimension + (nX + 0);
	int index2 = (nZ + 0) * m_dimension + (nX + 1);
	int index3 = (nZ + 1) * m_dimension + (nX + 1);

	// x + y = 1의 직선의 방정식을 이용하라( y = -x + 1 )
	// 1-3 
	// |/|
	// 0-2
	// x + y < 1이면 012, x + y >= 1이면 123
	if (deltaX + deltaZ < 1.0f)
	{
		// 012 삼각형 사용
		float zY = m_vecVertex[index1].y - m_vecVertex[index0].y;
		float xY = m_vecVertex[index2].y - m_vecVertex[index0].y;

		height = m_vecVertex[index0].y + (zY * deltaZ) + (xY * deltaX);
	}
	else
	{
		deltaX = 1 - deltaX;
		deltaZ = 1 - deltaZ;

		// 123 삼각형 사용
		float zY = m_vecVertex[index1].y - m_vecVertex[index3].y;
		float xY = m_vecVertex[index2].y - m_vecVertex[index3].y;

		height = m_vecVertex[index3].y + (zY * deltaZ) + (xY * deltaX);
	}

	return true;
}

bool MHeightMap::CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY)
{
	// 당장에 쓸 일은 없지만 나중에 쓸 수도 있으므로 주석처리함
	// 
	// 좌표를 받은 후 world좌표로 옮긴 후 -> ray가 나옴
	// 그 ray의 값을 이용해서 vector의 포지션에서 가장 거리가 짧은 애를 찾아서
	// WndProc를 넘겨줘서 캐릭터가 그 pos로 갈 수 있게 한다
	//Ray r = Ray::RayAtWorldSpace(screenX, screenY);

	//float intersectDist;
	//float minDistance = FLT_MAX;

	//bool isInterect = false;

	//for (size_t i = 0; i < m_vecSurfaceVertex.size(); i += 3)
	//{
	//	if (r.CalcIntersectTri(&m_vecSurfaceVertex[i], &intersectDist))
	//	{
	//		if (intersectDist < minDistance)
	//		{
	//			minDistance = intersectDist;
	//			isInterect = true;

	//			// 클릭한 위치(m_pos)에서 바닥의 방향(m_dir) * 거리(intersectDist)를 더해줌
	//			vOut = r.m_pos + (r.m_dir * intersectDist);
	//		}
	//	}
	//}

	//return isInterect;
	return false;
}

void MHeightMap::SetSurface()
{
	// 타일맵 만드는 거와 똑같은 작업을 한다
	vector<D3DXVECTOR3> vecPos;
	vector<DWORD> vecIndex;

	int surfaceDim = 5;
	int numSurfaceTile = surfaceDim - 1;

	vecPos.reserve(surfaceDim * surfaceDim);

	for (size_t z = 0; z < surfaceDim; z++)
	{
		for (size_t x = 0; x < surfaceDim; x++)
		{
			// 원래의 index의 x z 값에 m_numTile(5)를 곱해 5 * 5의 vertex를 저장하게 한다
			DWORD index = ((z / (float)numSurfaceTile) * m_dimension) * m_numTile +
				(x / (float)numSurfaceTile) * m_numTile;

			vecPos.push_back(m_vecVertex[index]);
		}
	}

	// 한 면(Surface)당 두개의 삼각형(3개의 선) 필요 -> 2 * 3
	// Surface가 5 * 5개 필요하므로 5 * 5 * 2 * 3개가 필요하다
	vecIndex.reserve(numSurfaceTile * numSurfaceTile * 2 * 3);

	// 면(Surface)을 만든다!
	for (size_t z = 0; z < numSurfaceTile; z++)
	{
		for (size_t x = 0; x < numSurfaceTile; x++)
		{
			DWORD _0 = (z + 0) * surfaceDim + (x + 0);
			DWORD _1 = (z + 1) * surfaceDim + (x + 0);
			DWORD _2 = (z + 0) * surfaceDim + (x + 1);
			DWORD _3 = (z + 1) * surfaceDim + (x + 1);

			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);

			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
			vecIndex.push_back(_3);
		}
	}

	// 5 * 5 크기의 Surface들을 만듬
	for (size_t i = 0; i < vecIndex.size(); i++)
	{
		m_vecSurfaceVertex.push_back(vecPos[vecIndex[i]]);
	}
}


void MHeightMap::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	//case WM_RBUTTONUP:
	{
		//D3DXVECTOR3 pos;
		//if (CalcPickedPosition(pos, SCREEN_POINT(lParam)) == true)
		//{
		//	float num;
		//	GetHeight(num, pos);
		//	pos.y = num;
		//	// 캐릭터를 pos로 이동하게 하기
		//	//m_charPos = pos;
		//	static_cast<IDisplayObject*>(g_pPlayerMGR->GetPlayer(1).SetDestination(pos));
		//}
	}
	}
}
