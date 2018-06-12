#include "../stdafx.h"
#include "DXMap.h"
#include "../Obstacle/DMapObstacle.h"
#include "../Map/MHeightMap.h"
#include "../Map/Checkpoint.h"

DXMap::DXMap()
{
	// �⺻ raw���Ͽ� ���� Scaling
	// �ٸ� raw������ �ִٸ� �ű⿡ ���� �����ϸ��� ����
	D3DXMatrixScaling(&m_matScail, 0.2f, 0.03f, 0.2f);
}


DXMap::~DXMap()
{
	for (auto p : m_vecPobstacle)
		SAFE_RELEASE(p);

	//for (auto p : m_vecBuilding)
	//	SAFE_RELEASE(p);

	//for (auto p : m_vecItem)
	//	SAFE_RELEASE(p);

	SAFE_RELEASE(m_pHeightMap);
}

void DXMap::LoadDXMap(const char* DXMapfile)
{
	string _DXMapfile = DXMapfile;
	string fullPath = "Resource/Map/" + _DXMapfile;
	string MapName;

	m_pHeightMap = new MHeightMap;

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
		}
		else if (CompareStr(szToken, "size"))		// �� ������ ����
		{
			int mapSize;
			fin >> mapSize;
			m_pHeightMap->Setdimension(mapSize);
		}
		else if (CompareStr(szToken, "tex"))	// �ؽ��� ���� �̸� get
		{
			fin >> szToken;
			string TexfileName = szToken;
			string _TexPath = "Resource/Map/" + TexfileName;

			// mtl ���� ����!!!
			D3DMATERIAL9 mtl = DXUtil::WHITE_MTRL;
			m_pHeightMap->SetMtlTex(mtl, g_TextureMGR->GetTexture(_TexPath));
		}
		else if (CompareStr(szToken, "raw"))	// Raw ���� �̸� get
		{
			fin >> szToken;
			string RawfileName = szToken;
			string _RawPath = "Resource/Map/" + RawfileName;

			// raw������ �޾� Heightmap ����
			m_pHeightMap->Load(_RawPath.c_str(), &m_matScail);
			g_MapMGR->AddMap(MapName, m_pHeightMap);
		}
		else if (CompareStr(szToken, "obstacle")) // Obstacle get
		{
			size_t ObstacleNum;		// ���� ��ֹ� ����
			fin >> ObstacleNum;
			fin.get();				// ������ �ϳ� �����Ƿ� �����ش�

			// num ������ŭ obstacle�� �޾ƿ´�
			for (size_t i = 0; i < ObstacleNum; i++)
			{
				char ObstacleName[128];	// ��ֹ� �̸�
				char ObjFile[128];		// ��ֹ� obj���ϸ�
				D3DXVECTOR3 ObjPos;

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%s %f %f %f %s", ObstacleName, 128, &ObjPos.x, &ObjPos.y, &ObjPos.z, ObjFile, 128);

				// �Ľ��ؿ� ������ Obstacle�� �����Ѵ�
				DMapObstacle * Obs = new DMapObstacle;
				Obs->SetObstacle(ObjFile, ObjPos);
				Obs->Init();
				Obs->SetObstacleName(ObstacleName);

				// Obstacle ���Ϳ� �����Ѵ� 
				// -> �̺κ��� DisplayeObjMgr�� ���ֱ� ������ 
				// -> LOD�� �浹 �� �� �ʿ��� ������ϴ� �۾����� �����Ƿ� �����Ѵ�!
				m_vecPobstacle.push_back(Obs);
			}
		}
		else if (CompareStr(szToken, "building")) // building get
		{
			size_t BuildingNum;		// ���� �ǹ� ����
			fin >> BuildingNum;
			fin.get();				// ������ �ϳ� �����Ƿ� �����ش�

			// num ������ŭ building�� �޾ƿ´�
			for (size_t i = 0; i < BuildingNum; i++)
			{
				D3DXVECTOR3 BuildingPos;
				char BuildingName[128];	// �ǹ� �̸�
				char ObjFile[128];		// �ǹ� obj���ϸ�

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%s %f %f %f %s", BuildingName, 128, 
					&BuildingPos.x, &BuildingPos.y, &BuildingPos.z, ObjFile, 128);

				// �Ľ��ؿ� ������ building�� �����Ѵ�
				DMapObstacle * Bd = new DMapObstacle;	// �ӽ� �ڵ�
				//Building Bd = new Building;
				Bd->SetObstacle(ObjFile, BuildingPos);
				Bd->Init();
			}
		}
		else if (CompareStr(szToken, "item")) // item get
		{
			size_t ItemNum;
			fin >> ItemNum;
			fin.get();				// ������ �ϳ� �����Ƿ� �����ش�

			// num ������ŭ item�� �޾ƿ´�
			for (size_t i = 0; i < ItemNum; i++)
			{
				D3DXVECTOR3 ItemPos;
				int Eitemtype;		// ������ ����
 				char ObjFile[128];	// ������ obj���ϸ�

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%d %f %f %f %s", &Eitemtype, &ItemPos.x, &ItemPos.y, &ItemPos.z, ObjFile, 128);

				// �Ľ��ؿ� ������ item�� �����Ѵ�
				DMapObstacle * it = new DMapObstacle;	// �ӽ� �ڵ�
				//item it = new item;
				it->SetObstacle(ObjFile, ItemPos);
				it->Init();
			}
		}
		else if (CompareStr(szToken, "playerstart")) // �÷��̾� ���� ���� get
		{
			size_t PlayerNum;		// �÷��̾� ��
			fin >> PlayerNum;
			fin.get();				// ������ �ϳ� �����Ƿ� �����ش�

			// �÷��̾��� ���������� �޾ƿ´�!
			for (int i = 0; i < PlayerNum; i++)
			{
				D3DXVECTOR3 PlayerStartPos;
				int nowPlayer;

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%d %f %f %f", &nowPlayer, &PlayerStartPos.x, &PlayerStartPos.y, &PlayerStartPos.z);

				// playerManager ��� �÷��̾� ���������� �����ÿ� ���⼭ ���ֱ� �ٶ�
			}
		}
		else if (CompareStr(szToken, "checkpoint"))	// üũ����Ʈ get
		{
			size_t CheckpointNum;	// üũ����Ʈ ��
			fin >> CheckpointNum;
			fin.get();				// ������ �ϳ� �����Ƿ� �����ش�

			// üũ����Ʈ ������ �޾ƿ´�!
			for (int i = 0; i < CheckpointNum; i++)
			{
				D3DXVECTOR3 CheckpointPos;
				int nowCheckpoint;

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%d %f %f %f", &nowCheckpoint, &CheckpointPos.x, &CheckpointPos.y, &CheckpointPos.z);
				
				// üũ����Ʈ ���� ����
				Checkpoint * CP = new Checkpoint();
				CP->Init();

				m_vecCheckpoint.push_back(CP);
			}

			// üũ����Ʈ ���� ������ŭ enemyspawn ����Ʈ ����
			g_MapMGR->SetEnemyspawnList(CheckpointNum);
		}
		else if (CompareStr(szToken, "enemyspawn"))	// �� �������� get
		{
			size_t EnemyspawnNum;	// �� ���� ��
			fin >> EnemyspawnNum;
			fin.get();				// ������ �ϳ� �����Ƿ� �����ش�

			// ���� ���������� �޾ƿ´�!
			for (int i = 0; i < EnemyspawnNum; i++)
			{
				D3DXVECTOR3 EnemySpawnPos;
				int nearCheckpoint;		// üũ����Ʈ�� ���ӵǴ� ���������̹Ƿ� üũ����Ʈ�� �Է¹���

				fin.getline(szToken, 128);
				sscanf_s(szToken, "%d %f %f %f", &nearCheckpoint, &EnemySpawnPos.x, &EnemySpawnPos.y, &EnemySpawnPos.z);

				// �� �������� ���� -> MapManager�� m_vecEnemyspawnList���� ������ �����Ƿ� ���� �Լ� �̿�
				g_MapMGR->PushEnemyspawn(nearCheckpoint - 1, EnemySpawnPos);
			}
		}
	}
}

void DXMap::Init()
{
	m_pHeightMap->Init();
}

void DXMap::Update()
{
}

void DXMap::Render()
{
	SAFE_RENDER(m_pHeightMap);

	for (auto p : m_vecPobstacle)
		SAFE_RENDER(p);
}

bool DXMap::GetHeight(OUT float & height, const D3DXVECTOR3 & pos)
{
	return m_pHeightMap->GetHeight(height, pos);
}

bool DXMap::CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY)
{
	return false;
}


// private��
bool DXMap::CompareStr(char * str1, const char * str2)
{
	return strcmp(str1, str2) == 0;
}
