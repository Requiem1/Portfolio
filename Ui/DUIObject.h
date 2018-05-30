#pragma once
#include "../Base/IDisplayObject.h"

class DUIObject :public IDisplayObject
{
protected:
	LPD3DXSPRITE m_Psprite;
public:
	D3DXVECTOR3 m_combinedPos;
	D3DXVECTOR3 m_pivot;
	D3DXVECTOR2 m_size;
	D3DCOLOR	m_color;

	bool m_bDrawBorder; // ī�޶� �տ� ������ ����, �������� ���


	DUIObject(LPD3DXSPRITE Psprite);
	virtual~DUIObject();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void DrawBorder();
	void UpdateCombinedPosition();// �θ�� �ڽ��� �������� ��ģ ī�޶� ������ ����
	void GetFinalRect(RECT* rect);
	const D3DXVECTOR3& GetCombinedPosition() { return m_combinedPos; }
};

