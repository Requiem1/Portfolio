#pragma once
#include "../Base/IDisplayObject.h"

class DSkinnedMesh : public IDisplayObject
{
protected:
	LPD3DXFRAME					m_pRootFrame;

	LPD3DXFRAME					m_pConnect;			// ��ü �߸��� �κ�!
	LPD3DXFRAME					m_pRootFrame_Up;	// ��ü ��Ʈ

	// Player�� ��ü ��Ʈ ������ = pelvis
	// Player�� ��ü ��Ʈ ������ = spine_01 
	// ��ü�� pelvis�� spine_01�� ������ ��ü�̴�

	// �ִϸ��̼� ��Ʈ�ѷ� -> �Ʒ�/��
	LPD3DXANIMATIONCONTROLLER	m_pAnimController_Down;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController_Up;
	
	int							m_EanimIndex_Down;
	int							m_EanimIndex_Up;

	float						m_fBlendTime;
	float						m_fPassedBlendTime;

	LPD3DXMESH					m_pSphereMesh;
	bool						m_bWireFrame;
	bool						m_bDrawFrame;
	bool						m_bDrawSkeleton;
	int							m_numFrame;
	int							m_numMesh;

	// �����̸�����
	CString						m_filePath;
	CString						m_fileName;

	map<LPCSTR, LPD3DXFRAME>	m_vecBonelist;		// ��ü Bone����Ʈ

	D3DXMATRIXA16				m_HandFrame_R;		// hand_r ������
	D3DXMATRIXA16				m_HandFrame_L;		// hand_l ������


public:
	DSkinnedMesh();
	virtual ~DSkinnedMesh();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void Load(LPCTSTR path, LPCTSTR filename);
private:
	void SetupBoneMatrixPointers(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);

	// Bone list�� ������ִ� �޼���
	void InitBonelist(LPD3DXFRAME pFrame);
	//void InitUpperBonelist(LPD3DXFRAME pFrame);
	//void InitLowerBonelist(LPD3DXFRAME pFrame);

	void UpdateAnim(LPD3DXANIMATIONCONTROLLER pAniController);
	void UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

	void DrawFrame(LPD3DXFRAME pFrame);
	void DrawMeshContainer(LPD3DXFRAME pFrame);
	void DrawSkeleton(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

protected:
	void SetAnimationIndex(int nIndex, bool isBlend, LPD3DXANIMATIONCONTROLLER pAniController);
	void DebugAnimationTime();
};

// OnInit
// pMeshContainerEx->ppBoneMatrixPtrs[i] = &pFrameExInfluence->CombinedTM;

// OnUpdate
// pFrameEx->CombinedTM = pFrameEx->TransformationMatrix * ((FRAME_EX*)pParent)->CombinedTM;
//
// pMeshContainerEx->pFinalBoneMatrices[i] =
// pMeshContainerEx->pBoneOffsetMatrices[i] * *(pMeshContainerEx->ppBoneMatrixPtrs[i]);
