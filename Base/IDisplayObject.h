#pragma once
#include "BaseObject.h"

class DrawingGroup;

class IDisplayObject : public BaseObject
{
protected:
	D3DXVECTOR3				m_pos;
	D3DXVECTOR3				m_rot;
	D3DXMATRIXA16			m_matWorld;
	IDisplayObject*			m_pParent;
	vector<IDisplayObject*> m_vecPChild;

	// 충돌 감지 바운싱 박스
	CBox * m_BoundingBox;

public:
	IDisplayObject();
	virtual ~IDisplayObject();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void AddChild(IDisplayObject* pChild);
	virtual void ReleaseAll();

	D3DXVECTOR3		GetPosition() { return m_pos; }
	void			SetPosition(D3DXVECTOR3* pos) { m_pos = *pos; }
	D3DXVECTOR3		GetRotation() { return m_rot; }
	D3DXMATRIXA16	GetWorldMatrix() { return m_matWorld; }

	// 0523 - 재익
	// 충돌 계산용 메서드 -> map file에서 불러온 오브젝트의 위치를 Set
	void SetObjectOnMap(D3DXVECTOR3 matWorld);

	CBox * GetCBox() { return m_BoundingBox; }
};

