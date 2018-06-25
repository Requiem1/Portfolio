#pragma once
#include "../Base/IDisplayObject.h"

class DSkinnedMesh : public IDisplayObject
{
protected:
	LPD3DXFRAME					m_pRootFrame;

	LPD3DXFRAME					m_pConnect;			// 상체 잘리는 부분!
	LPD3DXFRAME					m_pRootFrame_Up;	// 상체 루트

	// Player의 전체 루트 프레임 = pelvis
	// Player의 상체 루트 프레임 = spine_01 
	// 하체는 pelvis의 spine_01을 제외한 전체이다

	// 애니매이션 컨트롤러 -> 아래/위
	LPD3DXANIMATIONCONTROLLER	m_pAnimController_Down;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController_Up;
	
	int							m_EanimIndex_Down;
	int							m_EanimIndex_Up;

	bool						m_BAniLoop_Down;	// 애니매이션이 반복재생되는가
	bool						m_BAniLoop_Up;		// 애니매이션이 반복재생되는가


	float						m_fBlendTime;
	float						m_fPassedBlendTime;

	LPD3DXMESH					m_pSphereMesh;
	bool						m_bWireFrame;
	bool						m_bDrawFrame;
	bool						m_bDrawSkeleton;
	int							m_numFrame;
	int							m_numMesh;

	// 파일이름변수
	CString						m_filePath;
	CString						m_fileName;

	map<LPCSTR, LPD3DXFRAME>	m_vecBonelist;		// 전체 Bone리스트

	D3DXMATRIXA16				m_HandFrame_R;		// hand_r 프레임
	D3DXMATRIXA16				m_HandFrame_L;		// hand_l 프레임



	LPD3DVERTEXELEMENT9			 m_VertexDecl;		// x파일 인스턴싱용!
	IDirect3DVertexDeclaration9* m_pVertexDecl;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

public:
	DSkinnedMesh();
	virtual ~DSkinnedMesh();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void Load(LPCTSTR path, LPCTSTR filename);
private:
	void SetupBoneMatrixPointers(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);

	// RootFrame2를 찾아주는 메서드
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
