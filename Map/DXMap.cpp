#include "../stdafx.h"
#include "DXMap.h"
#include "../Obstacle/DMapObstacle.h"
#include "../Map/MHeightMap.h"
#include "../Map/Checkpoint.h"
#include "../Map/ObjMap.h"

DXMap::DXMap()
{
	// 기본 raw파일에 대한 Scaling
	// 다른 raw파일이 있다면 거기에 맞춰 스케일링을 하자
	D3DXMatrixScaling(&m_matScail, 0.5f, 0.05f, 0.5f);
	m_EmapType = HEIGHTMAP_MAPTYPE;

	m_pObjMap = NULL;
	m_pHeightMap = NULL;
}


DXMap::~DXMap()
{
	SAFE_RELEASE(m_pObjMap);
	SAFE_RELEASE(m_pHeightMap);

	for (auto p : m_vecPobstacle)
		SAFE_RELEASE(p);

	for (auto p : m_vecCheckpoint)
		SAFE_RELEASE(p);

	//for (auto p : m_vecBuilding)
	//	SAFE_RELEASE(p);

	//for (auto p : m_vecItem)
	//	SAFE_RELEASE(p);
}

void DXMap::LoadDXMap(const char* DXMapfile)
{
	string _DXMapfile = DXMapfile;
	string fullPath = "Resource/Map/" + _DXMapfile;
	string MapName;
	
	std::ifstream fin;
	fin.open(fullPath);

	char szToken[128];

	while (fin.eof() == false)
	{
		fin >> szToken;

		if (CompareStr(szToken, "map"))
		{
			fin >> szToken;
			MapName = szToken;

			// type
			fin >> szToken;	// type 구분자!
			fin >> szToken;

			if (CompareStr(szToken, "HeightMap"))
			{
				m_pHeightMap = new MHeightMap;
				m_EmapType = HEIGHTMAP_MAPTYPE;

				// size / tex / raw / mappos -> 4개
				for (int hmap = 0; hmap < 4; hmap++)
				{
					fin >> szToken;

					if (CompareStr(szToken, "size"))		// 맵 사이즈 설정
					{
						int mapSize;
						fin >> mapSize;
						m_pHeightMap->Setdimension(mapSize);
					}
					else if (CompareStr(szToken, "tex"))	// 텍스쳐 파일 이름 get
					{
						fin >> szToken;
						string TexfileName = szToken;
						string _TexPath = "Resource/Map/" + TexfileName;

						// mtl 파일 세팅!!!
						D3DMATERIAL9 mtl = DXUtil::WHITE_MTRL;
						m_pHeightMap->SetMtlTex(mtl, g_TextureMGR->GetTexture(_TexPath));
					}
					else if (CompareStr(szToken, "raw"))	// Raw 파일 이름 get
					{
						fin >> szToken;
						string RawfileName = szToken;
						string _RawPath = "Resource/Map/" + RawfileName;

						// raw파일을 받아 Heightmap 생성
						m_pHeightMap->Load(_RawPath.c_str(), &m_matScail);
						g_MapMGR->AddMap(MapName, m_pHeightMap);
						g_MapMGR->SetCurrentMap(MapName);
					}
					else if (CompareStr(szToken, "mappos"))
					{
						D3DXVECTOR3 mapPosition;

						fin.getline(szToken, 128);
						sscanf_s(szToken, "%f %f %f", &mapPosition.x, &mapPosition.y, &mapPosition.z);

						m_pHeightMap->SetPosition(&mapPosition);
					}
				}

			}
			else if (CompareStr(szToken, "ObjMap"))
			{
				m_pObjMap = new ObjMap;
				m_EmapType = OBJMAP_MAPTYPE;

				fin >> szToken;

				if(CompareStr(szToken, "objfile"))
				{
					fin >> szToken;
					m_pObjMap->SetObjfileName(szToken);
					m_pObjMap->Init();

					g_MapMGR->AddMap(MapName, m_pObjMap);
					g_MapMGR->SetCurrentMap(MapName);
				}
			}
		}

		else if (CompareStr(szToken, "obstacle")) // Obstacle get
		{
			size_t ObstacleNum;		// 맵의 장애물 개수
			fin >> ObstacleNum;
			fin.get();				// 공백이 하나 남으므로 날려준다

			// num 개수만큼 obstacle을 받아온다
			for (size_t i = 0; i < ObstacleNum; i++)
			{
				char ObstacleName[128];	// 장애물 이름
				char ObjFile[128];		// 장애물 obj파일명
				D3DXVECTOR3 ObjPos;

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%s %f %f %f %s", ObstacleName, 128, &ObjPos.x, &ObjPos.y, &ObjPos.z, ObjFile, 128);

				// 파싱해온 정보로 Obstacle을 생성한다
				DMapObstacle * Obs = new DMapObstacle;
				Obs->SetObstacle(ObjFile, ObjPos);
				Obs->Init();
				Obs->SetObstacleName(ObstacleName);

				// Obstacle 벡터에 저장한다 
				// -> 이부분은 DisplayeObjMgr이 해주긴 하지만 
				// -> LOD와 충돌 등 각 맵에서 해줘야하는 작업들이 있으므로 저장한다!
				m_vecPobstacle.push_back(Obs);
			}
		}
		else if (CompareStr(szToken, "building")) // building get
		{
			size_t BuildingNum;		// 맵의 건물 개수
			fin >> BuildingNum;
			fin.get();				// 공백이 하나 남으므로 날려준다

			// num 개수만큼 building을 받아온다
			for (size_t i = 0; i < BuildingNum; i++)
			{
				D3DXVECTOR3 BuildingPos;
				char BuildingName[128];	// 건물 이름
				char ObjFile[128];		// 건물 obj파일명

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%s %f %f %f %s", BuildingName, 128, 
					&BuildingPos.x, &BuildingPos.y, &BuildingPos.z, ObjFile, 128);

				// 파싱해온 정보로 building을 생성한다
				DMapObstacle * Bd = new DMapObstacle;	// 임시 코드
				//Building Bd = new Building;
				Bd->SetObstacle(ObjFile, BuildingPos);
				Bd->Init();
			}
		}
		else if (CompareStr(szToken, "item")) // item get
		{
			size_t ItemNum;
			fin >> ItemNum;
			fin.get();				// 공백이 하나 남으므로 날려준다

			// num 개수만큼 item을 받아온다
			for (size_t i = 0; i < ItemNum; i++)
			{
				D3DXVECTOR3 ItemPos;
				int Eitemtype;		// 아이템 종류
 				char ObjFile[128];	// 아이템 obj파일명

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%d %f %f %f %s", &Eitemtype, &ItemPos.x, &ItemPos.y, &ItemPos.z, ObjFile, 128);

				// 파싱해온 정보로 item을 생성한다
				DMapObstacle * it = new DMapObstacle;	// 임시 코드
				//item it = new item;
				it->SetObstacle(ObjFile, ItemPos);
				it->Init();
			}
		}
		else if (CompareStr(szToken, "playerstart")) // 플레이어 시작 지점 get
		{
			size_t PlayerNum;		// 플레이어 수
			fin >> PlayerNum;
			fin.get();				// 공백이 하나 남으므로 날려준다

			// 플레이어의 시작지점을 받아온다!
			for (int i = 0; i < PlayerNum; i++)
			{
				D3DXVECTOR3 PlayerStartPos;
				int nowPlayer;

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%d %f %f %f", &nowPlayer, &PlayerStartPos.x, &PlayerStartPos.y, &PlayerStartPos.z);

				// playerManager 등에서 플레이어 시작지점을 설정시에 여기서 해주기 바람
				g_PlayerMGR->AddStartPos(PlayerStartPos);
			}
		}
		else if (CompareStr(szToken, "checkpoint"))	// 체크포인트 get
		{
			size_t CheckpointNum;	// 체크포인트 수
			fin >> CheckpointNum;
			fin.get();				// 공백이 하나 남으므로 날려준다

			// 체크포인트 지점을 받아온다!
			for (int i = 0; i < CheckpointNum; i++)
			{
				D3DXVECTOR3 CheckpointPos;
				int nowCheckpoint;

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%d %f %f %f", &nowCheckpoint, &CheckpointPos.x, &CheckpointPos.y, &CheckpointPos.z);
				
				// 체크포인트 지점 설정
				Checkpoint * CP = new Checkpoint();
				CP->SetPosition(&CheckpointPos);
				CP->Init();

				m_vecCheckpoint.push_back(CP);
			}

			// 체크포인트 지점 개수만큼 enemyspawn 리스트 설정
			g_MapMGR->SetEnemyspawnList(CheckpointNum);
		}
		else if (CompareStr(szToken, "enemyspawn"))	// 적 스폰지점 get
		{
			size_t EnemyspawnNum;	// 적 스폰 수
			fin >> EnemyspawnNum;
			fin.get();				// 공백이 하나 남으므로 날려준다

			// 적의 스폰지점을 받아온다!
			for (int i = 0; i < EnemyspawnNum; i++)
			{
				D3DXVECTOR3 EnemySpawnPos;
				int nearCheckpoint;		// 체크포인트에 종속되는 스폰지점이므로 체크포인트를 입력받음

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%d %f %f %f", &nearCheckpoint, &EnemySpawnPos.x, &EnemySpawnPos.y, &EnemySpawnPos.z);

				// 적 스폰지점 설정 -> MapManager의 m_vecEnemyspawnList에서 가지고 있으므로 관련 함수 이용
				g_MapMGR->PushEnemyspawn(nearCheckpoint - 1, EnemySpawnPos);
			}
		}
	}
}

void DXMap::Init()
{

	switch (m_EmapType)
	{
	case HEIGHTMAP_MAPTYPE:	m_pHeightMap->Init();	break;
	case OBJMAP_MAPTYPE:	m_pObjMap->Init();		break;
	}
}

void DXMap::Update()
{
	for (auto p : m_vecCheckpoint)
		SAFE_UPDATE(p);

	//for (auto p : m_vecPobstacle)
	//	SAFE_UPDATE(p);
}

void DXMap::Render()
{
	for (auto p : m_vecCheckpoint)
		SAFE_RENDER(p);

	for (auto p : m_vecPobstacle)
		SAFE_RENDER(p);

	switch (m_EmapType)
	{
	case HEIGHTMAP_MAPTYPE:	SAFE_RENDER(m_pHeightMap);	break;
	case OBJMAP_MAPTYPE:	SAFE_RENDER(m_pObjMap);		break;
	}
}

bool DXMap::GetHeight(OUT float & height, const D3DXVECTOR3 & pos)
{
	switch (m_EmapType)
	{
	case HEIGHTMAP_MAPTYPE:	return m_pHeightMap->GetHeight(height, pos);
	case OBJMAP_MAPTYPE:	return m_pObjMap->GetHeight(height, pos);
	}
}

bool DXMap::CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY)
{
	return false;
}


// private임
bool DXMap::CompareStr(char * str1, const char * str2)
{
	return strcmp(str1, str2) == 0;
}
