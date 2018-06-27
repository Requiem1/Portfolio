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

// DXMap -> HeightMap Ŭ������ ������ Ŭ����
// DXMap�� ������ �ڵ� �ϴ� �ּ� ����
class DXMap : public IMap
{
protected:
	MHeightMap *	m_pHeightMap;	// raw�� �̿��� ���̸�
	ObjMap *		m_pObjMap;		// Obj �ε带 �޾ƿ��� Obj��

	int				m_EmapType;		// obj�ΰ� heightmap�ΰ�?

	D3DXMATRIXA16	m_matScail;		// �� ũ�� ������ ��Ʈ����
	
	vector<DMapObstacle*>	m_vecPobstacle;	// ���� ��ֹ���
	//vector<DBuilding*>	m_vecBuilding	// ĳ���Ͱ� �ȿ� �� �� �ִ� �ǹ�
	//vector<item*>			m_vecItem;		// ������

	vector<Checkpoint*>		m_vecCheckpoint;// üũ����Ʈ

public:
	DXMap();
	~DXMap();

	// �� �ε� �Լ�
	// DXMapfile : DXMap ���� ��
	// �⺻������ ��δ� Resource/Map/ �Ʒ��� �����Ǿ�����
	void LoadDXMap(const char* DXMapfile);

	// IMap��(��) ���� ��ӵ�
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
 -- DXMap�� Scene������ ���� --
 m_PdxMap = new DXMap;
 m_PdxMap->LoadDXMap("SampleObjMap.DXMap");
 m_PdxMap->Init();
 AddSimpleDisplayObj(m_PdxMap);
*/

/*
 * ������ ����!!!
 * Ȯ���ڴ� .DXMap
 --- HeightMap�� ��� ---
���� ����
map mapname					// ���� �̸�
type Heightmap				// �� Ÿ�� -> heightmap!
size num					// ���� ������ -> num * num
tex filename.jpg			// ���� �ؽ�������
raw filename.raw			// ����  raw����(����)

obstacle num				// num : ��ֹ� ��
obsname x y z filename.obj	// ��ֹ� �̸�, ������ǥ, obj���ϸ�
...

building num				// num : ���� ��
bdname x y z filename.obj	// �ǹ� �̸�, ������ǥ, obj���ϸ�
...

item num					// num : ������ ��
itemEnum x y z filename.obj	// ������ ����, ������ǥ, obj���ϸ�
...

playerstart	num				// num : �÷��̾��� ��
1 x y z						// 1 2 3 4 �÷��̾��� ��������
2 x y z
3 x y z
4 x y z

checkpoint num				// num : üũ����Ʈ ���� ��
1 x y z						// 1�� üũ����Ʈ�� x y z
2 x y z
3 x y z
4 x y z
5 x y z

enemyspawn num				// num : ���� ������ġ ����
1 x y z						// 1��üũ����Ʈ�� ���� ������ġ
1 x y z						// üũ����Ʈ���� �������� ������ġ�� ����
1 x y z
2 x y z
2 x y z
...
*/

/*
 --- ObjMap�� ��� ---
 map mapname				// ���� �̸�
 type ObjMap				// �� Ÿ�� -> ObjMap
 objfile objname.obj		// obj���� �̸�

 -- �ϴ��� Heightmap�� ���� --
*/

/*
SampleMap.DXMap�� ����

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