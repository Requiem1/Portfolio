#pragma once
#include "../Base/IDisplayObject.h"

class DSkinnedMesh : public IDisplayObject
{
protected:
	LPD3DXFRAME					m_pRootFrame;

	LPD3DXFRAME					m_pUpperRootFrame;	// 상체 루트 프레임
	LPD3DXFRAME					m_pLowerRootFrame;	// 하체 루트 프레임

	// Player의 전체 루트 프레임 = pelvis
	// Player의 상체 루트 프레임 = spine_01 
	// 하체는 pelvis의 spine_01을 제외한 전체이다

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	
	int							m_EanimIndex;
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

	// Bone list를 만들어주는 메서드
	void InitBonelist(LPD3DXFRAME pFrame);
	void InitUpperBonelist(LPD3DXFRAME pFrame);
	void InitLowerBonelist(LPD3DXFRAME pFrame);

	void UpdateAnim();
	void UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

	void DrawFrame(LPD3DXFRAME pFrame);
	void DrawMeshContainer(LPD3DXFRAME pFrame);
	void DrawSkeleton(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

public:
	void SetAnimationIndex(int nIndex, bool isBlend);
};

// OnInit
// pMeshContainerEx->ppBoneMatrixPtrs[i] = &pFrameExInfluence->CombinedTM;

// OnUpdate
// pFrameEx->CombinedTM = pFrameEx->TransformationMatrix * ((FRAME_EX*)pParent)->CombinedTM;
//
// pMeshContainerEx->pFinalBoneMatrices[i] =
// pMeshContainerEx->pBoneOffsetMatrices[i] * *(pMeshContainerEx->ppBoneMatrixPtrs[i]);
