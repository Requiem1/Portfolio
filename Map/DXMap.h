#pragma once
#include "../Map/IMap.h"

class MHeightMap;
class ObjMap;
class DMapObstacle;
class Checkpoint;

enum MAPTYPEENUM
{
	HEIGHTMAP_MAPTYPE,
	OBJMAP_MAPTYPE
};

// DXMap -> HeightMap 클래스를 가지는 클래스
// DXMap의 구조는 코드 하단 주석 참조
class DXMap : public IMap
{
protected:
	MHeightMap *	m_pHeightMap;	// raw를 이용한 높이맵
	ObjMap *		m_pObjMap;		// Obj 로드를 받아오는 Obj맵

	int				m_EmapType;		// obj인가 heightmap인가?

	D3DXMATRIXA16	m_matScail;		// 맵 크기 조절용 매트릭스
	
	vector<DMapObstacle*>	m_vecPobstacle;	// 맵의 장애물들
	//vector<DBuilding*>	m_vecBuilding	// 캐릭터가 안에 들어갈 수 있는 건물
	//vector<item*>			m_vecItem;		// 아이템

	vector<Checkpoint*>		m_vecCheckpoint;// 체크포인트

public:
	DXMap();
	~DXMap();

	// 맵 로드 함수
	// DXMapfile : DXMap 파일 명
	// 기본적으로 경로는 Resource/Map/ 아래로 설정되어있음
	void LoadDXMap(const char* DXMapfile);

	// IMap을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual bool GetHeight(OUT float & height, const D3DXVECTOR3 & pos) override;
	virtual bool CalcPickedPosition(D3DXVECTOR3 & vOut, WORD screenX, WORD screenY) override;

	MHeightMap* GetHeightMap() { return m_pHeightMap; }
	ObjMap* GetObjMap() { return m_pObjMap; }
	vector<DMapObstacle*> GetDXMapObstacle() { return m_vecPobstacle; }

private:
	bool CompareStr(char * str1, const char * str2);
};

/* 
 -- DXMap의 Scene에서의 사용법 --
 m_PdxMap = new DXMap;
 m_PdxMap->LoadDXMap("SampleObjMap.DXMap");
 m_PdxMap->Init();
 AddSimpleDisplayObj(m_PdxMap);
*/

/*
 * 맵파일 구조!!!
 * 확장자는 .DXMap
 --- HeightMap의 방식 ---
들어가는 내용
map mapname					// 맵의 이름
type Heightmap				// 맵 타입 -> heightmap!
size num					// 맵의 사이즈 -> num * num
tex filename.jpg			// 맵의 텍스쳐파일
raw filename.raw			// 맵의  raw파일(높이)

obstacle num				// num : 장애물 수
obsname x y z filename.obj	// 장애물 이름, 월드좌표, obj파일명
...

building num				// num : 빌딩 수
bdname x y z filename.obj	// 건물 이름, 월드좌표, obj파일명
...

item num					// num : 아이템 수
itemEnum x y z filename.obj	// 아이템 종류, 월드좌표, obj파일명
...

playerstart	num				// num : 플레이어의 수
1 x y z						// 1 2 3 4 플레이어의 시작지점
2 x y z
3 x y z
4 x y z

checkpoint num				// num : 체크포인트 지점 수
1 x y z						// 1번 체크포인트의 x y z
2 x y z
3 x y z
4 x y z
5 x y z

enemyspawn num				// num : 적의 스폰위치 개수
1 x y z						// 1번체크포인트의 적의 스폰위치
1 x y z						// 체크포인트마다 여러개의 스폰위치를 가짐
1 x y z
2 x y z
2 x y z
...
*/

/*
 --- ObjMap의 방식 ---
 map mapname				// 맵의 이름
 type ObjMap				// 맵 타입 -> ObjMap
 objfile objname.obj		// obj파일 이름

 -- 하단은 Heightmap과 같음 --
*/

/*
SampleMap.DXMap의 내용

map Samplemap1
size 257
tex desert.jpg
raw HeightMap.raw

obstacle 2
Switch_1 10.000000 1.000000 10.000000 Switch.obj
Wall_1 10.000000 1.000000 12.000000 wall2.obj

building 0

item 0

playerstart 1
1 1.000000 1.000000 1.000000

checkpoint 1
1 10.000000 1.000000 10.000000

enemyspawn 1
1 10.000000 1.000000 10.000000
*/