#include "../stdafx.h"
#include "DSkinnedMesh.h"
#include "AllocateHierarchy.h"

DSkinnedMesh::DSkinnedMesh()
{
	//m_Brot.y = ;
	//m_baseRotY = D3DX_PI;

	m_pRootFrame = NULL;
	m_pAnimController_Down = NULL;
	m_fBlendTime = 0.3f;
	m_fPassedBlendTime = 0.0f;

	m_EanimIndex_Down = 0;
	m_EanimIndex_Up = 0;

	m_BAniLoop_Up = true;
	m_BAniLoop_Down = true;

	m_bWireFrame = false;
	m_bDrawFrame = true;
	m_bDrawSkeleton = false;
}

DSkinnedMesh::~DSkinnedMesh()
{
	SAFE_RELEASE(m_pSphereMesh);
	AllocateHierarchy alloc;
	D3DXFrameDestroy(m_pRootFrame, &alloc);
	D3DXFrameDestroy(m_pRootFrame_Up, &alloc);

	SAFE_RELEASE(m_pAnimController_Down);
	SAFE_RELEASE(m_pAnimController_Up);
}

void DSkinnedMesh::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXCreateSphere(g_Device, 0.01f, 10, 10, &m_pSphereMesh, NULL);

	// 크기 설정!!
	Load(m_filePath, m_fileName);					// X파일 로드!

	
	// 상체와 하체를 분리하는 루트 프레임 찾기 -> spine_02
	FindRootFrame2(m_pRootFrame, "spine_02");
}

void DSkinnedMesh::Load(LPCTSTR path, LPCTSTR filename)
{
	AllocateHierarchy alloc(path);

	CString fullPath(path);
	fullPath.Append(filename);
	
	D3DXLoadMeshHierarchyFromX(fullPath, D3DXMESH_MANAGED, g_Device,
		&alloc, NULL, &m_pRootFrame, &m_pAnimController_Down);

	m_pAnimController_Down->CloneAnimationController(
		m_pAnimController_Down->GetMaxNumAnimationOutputs(),
		m_pAnimController_Down->GetMaxNumAnimationSets(),
		m_pAnimController_Down->GetMaxNumTracks(),
		m_pAnimController_Down->GetMaxNumEvents(),
		&m_pAnimController_Up
	);

	SetupBoneMatrixPointers(m_pRootFrame);
}

// 각 프레임의 매시 컨테이너에 있는 pSkinInfo를 이용하여 
// 현재 매쉬에 영향을 주는 프레임들의 월드행렬 포인터를 연결 
//Called to setup the pointers for a given bone to its transformation matrix
void DSkinnedMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		SetupBoneMatrixPointers(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
	}
}

void DSkinnedMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DWORD numBones;
	FRAME_EX* pFrameExInfluence;
	MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pMeshContainerBase;

	if (pMeshContainerEx->pSkinInfo != NULL)
	{
		numBones = pMeshContainerEx->pSkinInfo->GetNumBones();

		for (DWORD i = 0; i < numBones; ++i)
		{
			pFrameExInfluence = (FRAME_EX*)D3DXFrameFind(m_pRootFrame,
				pMeshContainerEx->pSkinInfo->GetBoneName(i));

			pMeshContainerEx->ppBoneMatrixPtrs[i] = &pFrameExInfluence->CombinedTM;
		}
	}
}

void DSkinnedMesh::FindRootFrame2(LPD3DXFRAME pFrame, string MiddleFrameName)
{
	m_vecBonelist[pFrame->Name] = pFrame;

	// 메쉬는 날려버린다!
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		m_vecBonelist.erase(pFrame->Name);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// 프레임!
	if (pFrame->pFrameSibling != NULL)
	{
		// 상체와 하체를 분리하는 루트 프레임 찾기 -> spine_02
		if (strcmp(pFrame->pFrameSibling->Name, MiddleFrameName.c_str()) == 0)
		{
			m_pConnect = pFrame;
			m_pRootFrame_Up = pFrame->pFrameSibling;

			pFrame->pFrameSibling = NULL;
			return;
		}

		FindRootFrame2(pFrame->pFrameSibling, MiddleFrameName);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		// 상체와 하체를 분리하는 루트 프레임 찾기 -> spine_02
		if (strcmp(pFrame->pFrameFirstChild->Name, MiddleFrameName.c_str()) == 0)
		{
			m_pConnect = pFrame;
			m_pRootFrame_Up = pFrame->pFrameFirstChild;

			pFrame->pFrameFirstChild = NULL;
			return;
		}

		FindRootFrame2(pFrame->pFrameFirstChild, MiddleFrameName);
	}
}

void DSkinnedMesh::Update()
{
	// 하체 Update
	// 애니매이션이 1개 이상이라면 애니매이션 동작
	if (m_pAnimController_Down != NULL && m_pAnimController_Down->GetMaxNumAnimationSets() > 1)
		SetAnimationIndex(m_EanimIndex_Down, true, m_pAnimController_Down);

	if(m_pAnimController_Down != NULL)
		UpdateAnim(m_pAnimController_Down);

	UpdateFrameMatrices(m_pRootFrame, NULL);


	// 상체 Update
	// m_rot을 사용해야하는 문제가 있음.. 플레이어 외에는 상체를 따로 작업하는 유닛이 없어서 괜찮긴 한데..
	if (m_pConnect != NULL)
	{
		FRAME_EX* pFrameEx = (FRAME_EX*)m_pConnect;

		// 애니매이션 동작
		if (m_pAnimController_Up != NULL && m_pAnimController_Up->GetMaxNumAnimationSets() > 1)
			SetAnimationIndex(m_EanimIndex_Up, true, m_pAnimController_Up);

		if (m_pAnimController_Up != NULL)
			UpdateAnim(m_pAnimController_Up);

		// 상체하체가 분리되었다면 상체부분의 rot을 따로 계산해준다.
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationX(&matR, m_rot.y);

		// 잘려진 부분의 TM을 계산해준다
		pFrameEx->CombinedTM = matR * ((FRAME_EX*)m_pConnect)->CombinedTM;

		// bone을 다시 계산한다
		UpdateFrameMatrices(m_pRootFrame_Up, pFrameEx);
	}
}


void DSkinnedMesh::UpdateAnim(LPD3DXANIMATIONCONTROLLER pAniController)
{
	float fDeltaTime = g_TimeMGR->GetDeltaTime();

	// AdvanceTime 함수가 호출된 간격으로 Anim 키프레임 계산
	pAniController->AdvanceTime(fDeltaTime, NULL);
	
	if (m_fPassedBlendTime <= m_fBlendTime)
	{
		m_fPassedBlendTime += fDeltaTime;

		if (m_fPassedBlendTime < m_fBlendTime)
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;

			pAniController->SetTrackWeight(0, fWeight);
			pAniController->SetTrackWeight(1, 1.0f - fWeight);
		}
		else
		{
			pAniController->SetTrackWeight(0, 1);
			pAniController->SetTrackWeight(1, 0);
			pAniController->SetTrackEnable(1, false);
		}
	}
}

void DSkinnedMesh::UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	// hand_r과 hand_l을 뽑아온다
	if (pFrame->Name != NULL && strcmp(pFrame->Name, "hand_r"))
	{
		FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;
		m_HandFrame_R = (pFrameEx->CombinedTM * m_matWorld);
	}
	else if (pFrame->Name != NULL && strcmp(pFrame->Name, "hand_l"))
	{
		FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;
		m_HandFrame_L = (pFrameEx->CombinedTM * m_matWorld);
	}


	FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;

	if (pParent != NULL)
	{
		pFrameEx->CombinedTM = pFrameEx->TransformationMatrix * ((FRAME_EX*)pParent)->CombinedTM;
	}
	else
	{
		pFrameEx->CombinedTM = pFrameEx->TransformationMatrix;
	}

	if (pFrameEx->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrameEx->pFrameSibling, pParent);
	}

	if (pFrameEx->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrameEx->pFrameFirstChild, pFrameEx);
	}
}


void DSkinnedMesh::Render()
{
	//if ( m_bDrawFrame)
	DrawFrame(m_pRootFrame);		// 전체
	//DrawFrame(m_pUpperRootFrame);	// 상체만
	//DrawFrame(m_pLowerRootFrame);	// 하체만

	// Bone은 그릴 필요 업음!
	if (m_bDrawSkeleton)
		DrawSkeleton(m_pRootFrame, NULL);	
}

// Desc: Called to render a frame in the hierarchy
void DSkinnedMesh::DrawFrame(LPD3DXFRAME pFrame)
{	
	// 매쉬!
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// 프레임!
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

void DSkinnedMesh::DrawMeshContainer(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer->pSkinInfo == NULL)
		return;

	FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;
	MESHCONTAINER_EX* pMeshContainerEx = (MESHCONTAINER_EX*)pFrameEx->pMeshContainer;
	DWORD numBones = pMeshContainerEx->pSkinInfo->GetNumBones();

	for (DWORD i = 0; i < numBones; ++i)
	{
		pMeshContainerEx->pFinalBoneMatrices[i] =
			pMeshContainerEx->pBoneOffsetMatrices[i] * *(pMeshContainerEx->ppBoneMatrixPtrs[i]);
	}

	PBYTE pVerticesSrc = NULL;
	PBYTE pVerticesDest = NULL;

	pMeshContainerEx->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);
	pMeshContainerEx->pWorkMesh->LockVertexBuffer(0, (LPVOID*)&pVerticesDest);

	// generate skinned mesh
	pMeshContainerEx->pSkinInfo->UpdateSkinnedMesh(
		pMeshContainerEx->pFinalBoneMatrices, NULL, pVerticesSrc, pVerticesDest);

	pMeshContainerEx->pWorkMesh->UnlockVertexBuffer();
	pMeshContainerEx->pOrigMesh->UnlockVertexBuffer();

	// 라이트 켬!
	g_Device->SetRenderState(D3DRS_LIGHTING, true);

	if (m_bWireFrame) 
		g_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// 월드 매트릭스 세팅 -> 움직이는 위치에 맞게 조작시킴
	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);



	//if(!m_pVertexDecl)
	//	g_Device->CreateVertexDeclaration(m_VertexDecl, &m_pVertexDecl);

	//g_Device->SetVertexDeclaration(m_pVertexDecl);

	//if (!m_pVB) m_pSphereMesh->GetVertexBuffer(&m_pVB);
	//if (!m_pIB) m_pSphereMesh->GetIndexBuffer(&m_pIB);

	////CheckInstanceDatabuffer(g_Device);

	//g_Device->SetStreamSource(0, m_pVB, 0, sizeof());



	//// ------------
	//// 쉐이더 계산!
	//// ------------
	//D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	//D3DXDeclaratorFromFVF(VERTEX_PNT::FVF, vertexDecl);

	//// 아래의 내용은 	D3DXDeclaratorFromFVF(VERTEX_PNT::FVF, vertexDecl);과 같은 결과를 낸다
	////D3DVERTEXELEMENT9 elements[] =
	////{
	////	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	////	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	////	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	////	D3DDECL_END()
	////};

	//LPDIRECT3DVERTEXDECLARATION9 pDecl = NULL;

	//// vertexDecl 대신 elements를 넣어도 똑같이 적용된다!
	//g_Device->CreateVertexDeclaration(vertexDecl, &pDecl);
	//g_Device->SetVertexDeclaration(pDecl);

	//UINT numPasses = 0;
	//m_pEffect->Begin(&numPasses, NULL);

	//for (int i = 0; i < numPasses; i++)
	//{
	//	m_pEffect->BeginPass(i);

	//	for (size_t i = 0; i < pMeshContainerEx->vecMtlTex.size(); ++i)
	//	{
	//		m_pEffect->SetTexture("DiffuseMap_Tex", pMeshContainerEx->vecMtlTex[i]->GetTexture());
	//		m_pEffect->CommitChanges();
	//		pMeshContainerEx->pWorkMesh->DrawSubset(i);
	//	}
	//}

	//m_pEffect->End();

	//UINT numPasses = 0;
	//m_pEffect->Begin(&numPasses, NULL);	// numPasses에 Pass의 개수를 가져옴
	//									// 한번 그릴때마다 Pass만큼 돌면서 그린다
	//for (int i = 0; i < numPasses; i++)
	//{
	//	m_pEffect->BeginPass(i);
	//	m_pMesh->DrawSubset(0);
	//	m_pEffect->EndPass();
	//}

	//m_pEffect->End();

	// shadermanager를 이용한다면 이걸 쓰지 않는다
	for (size_t i = 0; i < pMeshContainerEx->vecMtlTex.size(); ++i)
	{
		g_Device->SetMaterial(&pMeshContainerEx->vecMtlTex[i]->GetMaterial());
		g_Device->SetTexture(0, pMeshContainerEx->vecMtlTex[i]->GetTexture());
		pMeshContainerEx->pWorkMesh->DrawSubset(i);
	}
	
	g_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void DSkinnedMesh::DrawSkeleton(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	FRAME_EX* pFrameEx = (FRAME_EX*)pFrame;
	FRAME_EX* pParentFrameEx = (FRAME_EX*)pParent;

	g_Device->SetTransform(D3DTS_WORLD, &(pFrameEx->CombinedTM));
	//g_pDevice->SetTransform(D3DTS_WORLD, &(pFrameEx->CombinedTM * m_matWorld));
	
	g_Device->SetMaterial(&DXUtil::WHITE_MTRL);
	g_Device->SetTexture(0, NULL);
	m_pSphereMesh->DrawSubset(0);

	if (pParent != NULL && pFrame->Name != NULL && pParent->Name != NULL)
	{
		D3DXMATRIXA16 matThis = pFrameEx->CombinedTM;
		D3DXMATRIXA16 matParent = pParentFrameEx->CombinedTM;

		D3DXVECTOR3 posThis(matThis(3, 0), matThis(3, 1), matThis(3, 2));
		D3DXVECTOR3 posParent(matParent(3, 0), matParent(3, 1), matParent(3, 2));

		vector<VERTEX_PC> line{ VERTEX_PC(posThis, BLUE), VERTEX_PC(posParent, YELLOW) };

		// 라이트 끔!
		//g_Device->SetRenderState(D3DRS_LIGHTING, true);
		g_Device->SetFVF(VERTEX_PC::FVF);
		g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &line[0], sizeof(VERTEX_PC));

		// 라이트 다시 켬!
		//g_Device->SetRenderState(D3DRS_LIGHTING, true);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawSkeleton(pFrame->pFrameSibling, pParent);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawSkeleton(pFrame->pFrameFirstChild, pFrame);
	}
}


void DSkinnedMesh::SetAnimationIndex(int nIndex, bool isBlend, LPD3DXANIMATIONCONTROLLER pAniController)
{
	LPD3DXANIMATIONSET pNextAnimSet = NULL;
	pAniController->GetAnimationSet(nIndex, &pNextAnimSet);

	if (isBlend)
	{
		LPD3DXANIMATIONSET pPrevAnimSet = NULL;

		pAniController->GetTrackAnimationSet(0, &pPrevAnimSet);
		pAniController->SetTrackAnimationSet(1, pPrevAnimSet);

		D3DXTRACK_DESC trackDesc;
		pAniController->GetTrackDesc(0, &trackDesc);
		pAniController->SetTrackDesc(1, &trackDesc);
		
		pAniController->SetTrackWeight(0, 0.0f);
		pAniController->SetTrackWeight(1, 1.0f);

		SAFE_RELEASE(pPrevAnimSet);

		m_fPassedBlendTime = 0.0f;
	}

	pAniController->SetTrackAnimationSet(0, pNextAnimSet);
	pAniController->ResetTime();
	
	SAFE_RELEASE(pNextAnimSet);
}

void DSkinnedMesh::DebugAnimationTime()
{
	// 하체 애니메이션 출력
	D3DXTRACK_DESC track;
	LPD3DXANIMATIONSET pCurrAnimSet = NULL;
	m_pAnimController_Down->GetTrackDesc(m_EanimIndex_Down, &track);
	m_pAnimController_Down->GetAnimationSet(m_EanimIndex_Down, &pCurrAnimSet);

	Debug->AddText("Animation_Down : ");
	Debug->AddText(pCurrAnimSet->GetPeriod());							//전체 시간
	Debug->AddText(" / ");
	Debug->AddText(pCurrAnimSet->GetPeriodicPosition(track.Position));	//현재 시간 
	Debug->EndLine();

	// 루프되지 않는 애니매이션이라면 초기화시킨다
	if ( m_BAniLoop_Down == false && (pCurrAnimSet->GetPeriod() - 0.01f > pCurrAnimSet->GetPeriodicPosition(track.Position)) )
	{
		// 만일 애니매이션을 사용 후에 초기화해야될 경우 (ex) 붕대감기
		// 애니매이션을 바꿔준 후, 방금 쓴 프레임을 초기화해줘야한다
		//m_pAnimController_Down->SetTrackPosition(트랙, 애니매이션 프레임);
		m_pAnimController_Down->SetTrackPosition(m_EanimIndex_Down, 0);
		SetAnimationIndex(0, true, m_pAnimController_Down);		
	}


	// 상체 애니매이션 출력
	pCurrAnimSet = NULL;
	m_pAnimController_Up->GetTrackDesc(m_EanimIndex_Up, &track);
	m_pAnimController_Up->GetAnimationSet(m_EanimIndex_Up, &pCurrAnimSet);

	Debug->AddText("Animation_Up : ");
	Debug->AddText(pCurrAnimSet->GetPeriod());							//전체 시간
	Debug->AddText(" / ");
	Debug->AddText(pCurrAnimSet->GetPeriodicPosition(track.Position));	//현재 시간
	Debug->EndLine();

	// 루프되지 않는 애니매이션이라면 초기화시킨다
	if (m_BAniLoop_Up == false && (pCurrAnimSet->GetPeriod() - 0.01f > pCurrAnimSet->GetPeriodicPosition(track.Position)))
	{
		m_pAnimController_Down->SetTrackPosition(m_EanimIndex_Up, 0);
		SetAnimationIndex(0, true, m_pAnimController_Up);
	}

	pCurrAnimSet->Release();
}
