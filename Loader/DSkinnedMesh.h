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

	bool						m_BAniLoop_Down;	// �ִϸ��̼��� �ݺ�����Ǵ°�
	bool						m_BAniLoop_Up;		// �ִϸ��̼��� �ݺ�����Ǵ°�


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



	LPD3DVERTEXELEMENT9			 m_VertexDecl;		// x���� �ν��Ͻ̿�!
	IDirect3DVertexDeclaration9* m_pVertexDecl;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

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

	// RootFrame2�� ã���ִ� �޼���
	void FindRootFrame2(LPD3DXFRAME pFrame, string MiddleFrameName);

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
