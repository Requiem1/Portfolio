#include "../stdafx.h"
#include "DSkinnedMesh.h"
#include "AllocateHierarchy.h"

DSkinnedMesh::DSkinnedMesh()
{
	//m_Brot.y = ;
	//m_baseRotY = D3DX_PI;

	m_pRootFrame = NULL;
	m_pAnimController = NULL;
	m_fBlendTime = 0.3f;
	m_fPassedBlendTime = 0.0f;
	m_EanimIndex = 0;
	m_bWireFrame = false;
	m_bDrawFrame = true;
	m_bDrawSkeleton = false;
}

DSkinnedMesh::~DSkinnedMesh()
{
	SAFE_RELEASE(m_pSphereMesh);
	AllocateHierarchy alloc;
	D3DXFrameDestroy(m_pRootFrame, &alloc);

	SAFE_RELEASE(m_pAnimController);
}

void DSkinnedMesh::Init()
{
	//g_pKeyboardManager->SetMovingTarget(&m_keyState);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXCreateSphere(g_Device, 0.01f, 10, 10, &m_pSphereMesh, NULL);
	//D3DXMatrixScaling(&m_matWorld, 0.2, 0.2, 0.2);

	Load(m_filePath, m_fileName);

	m_BoundingBox->initBoundingBox(m_pSphereMesh);

	// m_vecBonelist�� bone���� �ִ´�!
	InitBonelist(m_pRootFrame);
}

void DSkinnedMesh::Load(LPCTSTR path, LPCTSTR filename)
{
	AllocateHierarchy alloc(path);

	CString fullPath(path);
	fullPath.Append(filename);
	
	D3DXLoadMeshHierarchyFromX(fullPath, D3DXMESH_MANAGED, g_Device,
		&alloc, NULL, &m_pRootFrame, &m_pAnimController);

	SetupBoneMatrixPointers(m_pRootFrame);
}

// �� �������� �Ž� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� 
// ���� �Ž��� ������ �ִ� �����ӵ��� ������� �����͸� ���� 
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

void DSkinnedMesh::InitBonelist(LPD3DXFRAME pFrame)
{
	m_vecBonelist[pFrame->Name] = pFrame;

	// �޽��� ����������!
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		m_vecBonelist.erase(pFrame->Name);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// ������!
	if (pFrame->pFrameSibling != NULL)
	{
		InitBonelist(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		InitBonelist(pFrame->pFrameFirstChild);
	}


	//// Player�� ��ü ��Ʈ ������ = spine_01 
	//// ��ü�� pelvis�� spine_01�� ������ ��ü�̴�
	//if (pFrame->Name != NULL && !strcmp(pFrame->Name, "spine_01"))
	//{
	//	// spine_01�� ��ü ��Ʈ �������� �ȴ�
	//	m_pUpperRootFrame = pFrame->pFrameFirstChild;
	//	InitUpperBonelist(pFrame->pFrameFirstChild);
	//	
	//	// �� ������ ��ü ��Ʈ �������� �ȴ�
	//	m_pLowerRootFrame = pFrame->pFrameSibling;
	//	InitLowerBonelist(pFrame->pFrameSibling);

	//	// ��ü�� ��ü �������� �����Ѵ�
	//	//pFrame->pFrameSibling = NULL;
	//}
}

void DSkinnedMesh::Update()
{	
	// �ִϸ��̼��� 1�� �̻��̶��
	if (m_pAnimController != NULL && m_pAnimController->GetMaxNumAnimationSets() > 1)
	{
		//if (GetAsyncKeyState(VK_F1) & 0x0001)
		//	m_EanimIndex = 0;
		//else if (GetAsyncKeyState(VK_F2) & 0x0001)
		//	m_EanimIndex = 1;

		// Enum�� ���ÿ���
		// m_EanimIndex�� Enum������ �������ֱ⸸ �ϸ� �ȴ�
		SetAnimationIndex(m_EanimIndex, true);
	}

	//if (GetAsyncKeyState(VK_F4) & 0x0001)
	//{
	//	m_bWireFrame = !m_bWireFrame;
	//	m_bDrawSkeleton = !m_bDrawSkeleton;
	//}

	if(m_pAnimController != NULL)
		UpdateAnim();

	UpdateFrameMatrices(m_pRootFrame, NULL);
	//UpdateFrameMatrices(m_pUpperRootFrame, NULL);
	//UpdateFrameMatrices(m_pLowerRootFrame, NULL);

	// �ٿ���ڽ� ������Ʈ
	m_BoundingBox->UpdateBoundingBox(m_matWorld, m_pos);
}


void DSkinnedMesh::UpdateAnim()
{
	float fDeltaTime = g_TimeMGR->GetDeltaTime();

	// AdvanceTime �Լ��� ȣ��� �������� Anim Ű������ ���
	m_pAnimController->AdvanceTime(fDeltaTime, NULL);
	
	if (m_fPassedBlendTime <= m_fBlendTime)
	{
		m_fPassedBlendTime += fDeltaTime;

		if (m_fPassedBlendTime < m_fBlendTime)
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;

			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		}
		else
		{
			m_pAnimController->SetTrackWeight(0, 1);
			m_pAnimController->SetTrackWeight(1, 0);
			m_pAnimController->SetTrackEnable(1, false);
		}
	}
}

void DSkinnedMesh::UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
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
		// ��ü�� �ƴ϶��!!
		//if(m_vecLowerBonelist.find(pFrameEx->pFrameSibling) == m_vecLowerBonelist.end())
			UpdateFrameMatrices(pFrameEx->pFrameSibling, pParent);
	}

	if (pFrameEx->pFrameFirstChild != NULL)
	{
		// ��ü�� �ƴ϶��!!
		//if (m_vecLowerBonelist.find(pFrameEx->pFrameSibling) == m_vecLowerBonelist.end())
			UpdateFrameMatrices(pFrameEx->pFrameFirstChild, pFrameEx);
	}
}


void DSkinnedMesh::Render()
{
	//m_numFrame = 0;
	//m_numMesh = 0;

	//if ( m_bDrawFrame)
	DrawFrame(m_pRootFrame);		// ��ü
	//DrawFrame(m_pUpperRootFrame);	// ��ü��
	//DrawFrame(m_pLowerRootFrame);	// ��ü��

	// �ٿ���ڽ� ����
	m_BoundingBox->RenderBoundingBox();

	// Bone�� �׸� �ʿ� ����!
	if (m_bDrawSkeleton)
		DrawSkeleton(m_pRootFrame, NULL);	
}

// Desc: Called to render a frame in the hierarchy
void DSkinnedMesh::DrawFrame(LPD3DXFRAME pFrame)
{
	//m_numFrame++;
	//if (pFrame->Name == NULL)
	//	Debug->AddText(_T("NULL"));
	//else
	//	Debug->AddText(pFrame->Name);
	
	// �Ž�!
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		//m_numMesh++;
		//Debug->AddText(_T("(MESH)"));
		DrawMeshContainer(pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// ������!
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

	// ����Ʈ ��!
	//g_Device->SetRenderState(D3DRS_LIGHTING, true);

	if (m_bWireFrame) 
		g_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// ���� ��Ʈ���� ���� -> �����̴� ��ġ�� �°� ���۽�Ŵ
	//D3DXMatrixScaling(&m_matWorld, 0.5, 0.5, 0.5);	// ũ�� ����!
	g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);

	//// ------------
	//// ���̴� ���!
	//// ------------
	//D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	//D3DXDeclaratorFromFVF(VERTEX_PNT::FVF, vertexDecl);

	//// �Ʒ��� ������ 	D3DXDeclaratorFromFVF(VERTEX_PNT::FVF, vertexDecl);�� ���� ����� ����
	////D3DVERTEXELEMENT9 elements[] =
	////{
	////	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	////	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	////	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	////	D3DDECL_END()
	////};

	//LPDIRECT3DVERTEXDECLARATION9 pDecl = NULL;

	//// vertexDecl ��� elements�� �־ �Ȱ��� ����ȴ�!
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

		// ����Ʈ ��!
		g_Device->SetRenderState(D3DRS_LIGHTING, false);
		g_Device->SetFVF(VERTEX_PC::FVF);

		// ���� ��Ʈ����
		//D3DXMATRIXA16 mat;
		//D3DXMatrixIdentity(&mat);
		//D3DXMatrixScaling(&mat, 0.5, 0.5, 0.5);	// ũ�� ����!

		g_Device->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &line[0], sizeof(VERTEX_PC));

		// ����Ʈ �ٽ� ��!
		g_Device->SetRenderState(D3DRS_LIGHTING, true);
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


void DSkinnedMesh::SetAnimationIndex(int nIndex, bool isBlend)
{
	LPD3DXANIMATIONSET pNextAnimSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pNextAnimSet);

	if (isBlend)
	{
		LPD3DXANIMATIONSET pPrevAnimSet = NULL;

		m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
		m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);

		D3DXTRACK_DESC trackDesc;
		m_pAnimController->GetTrackDesc(0, &trackDesc);
		m_pAnimController->SetTrackDesc(1, &trackDesc);
		
		m_pAnimController->SetTrackWeight(0, 0.0f);
		m_pAnimController->SetTrackWeight(1, 1.0f);

		SAFE_RELEASE(pPrevAnimSet);

		m_fPassedBlendTime = 0.0f;
	}

	m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
	m_pAnimController->ResetTime();
	
	SAFE_RELEASE(pNextAnimSet);
}
