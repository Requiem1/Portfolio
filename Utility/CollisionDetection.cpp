#pragma once

#include <math.h>
#include "../stdafx.h"
#include "CollisionDetection.h"

void CBox::MakeBoundingBox(CBox * pBox, D3DXVECTOR3 vecMin, D3DXVECTOR3 vecMax, D3DXVECTOR3 ObjPos)
{
	// Boundingbox를 렌더하기 위한 선을 만든다
	vector<VERTEX_PC> vecBoxPoint;
	D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);

	vecMin = vecMin - ObjPos;
	vecMax = vecMax - ObjPos;

	vecBoxPoint.push_back(VERTEX_PC(D3DXVECTOR3(vecMin.x, vecMin.y, vecMin.z), green)); //Min
	vecBoxPoint.push_back(VERTEX_PC(D3DXVECTOR3(vecMin.x, vecMax.y, vecMin.z), green));
	vecBoxPoint.push_back(VERTEX_PC(D3DXVECTOR3(vecMax.x, vecMax.y, vecMin.z), green));
	vecBoxPoint.push_back(VERTEX_PC(D3DXVECTOR3(vecMax.x, vecMin.y, vecMin.z), green));

	vecBoxPoint.push_back(VERTEX_PC(D3DXVECTOR3(vecMin.x, vecMin.y, vecMax.z), green));
	vecBoxPoint.push_back(VERTEX_PC(D3DXVECTOR3(vecMin.x, vecMax.y, vecMax.z), green));
	vecBoxPoint.push_back(VERTEX_PC(D3DXVECTOR3(vecMax.x, vecMax.y, vecMax.z), green)); //Max
	vecBoxPoint.push_back(VERTEX_PC(D3DXVECTOR3(vecMax.x, vecMin.y, vecMax.z), green));

	vecMin = vecMin + ObjPos;
	vecMax = vecMax + ObjPos;


	for (size_t i = 0; i < CUBE_INDEX_SIZE; i++)
	{
		pBox->m_vecBoxVertex.push_back(vecBoxPoint[g_aCubeIndex[i]]);
	}


	// BoundingBox의 연산용 변수들의 값을 넣는다
	pBox->center[0] = (vecMin.x + vecMax.x) / 2.0F;
	pBox->center[1] = (vecMin.y + vecMax.y) / 2.0F;
	pBox->center[2] = (vecMin.z + vecMax.z) / 2.0F;

	pBox->extent[0] = vecMax.x - pBox->center[0];
	pBox->extent[1] = vecMax.y - pBox->center[1];
	pBox->extent[2] = vecMax.z - pBox->center[2];

	// identity world coordinate axis
	pBox->axis[0][0] = 1.0F;
	pBox->axis[0][1] = 0.0F;
	pBox->axis[0][2] = 0.0F;

	pBox->axis[1][0] = 0.0F;
	pBox->axis[1][1] = 1.0F;
	pBox->axis[1][2] = 0.0F;

	pBox->axis[2][0] = 0.0F;
	pBox->axis[2][1] = 0.0F;
	pBox->axis[2][2] = 1.0F;

	pBox->translation[0] = 0.0F;
	pBox->translation[1] = 0.0F;
	pBox->translation[2] = 0.0F;
}


void CBox::initBoundingBox(ID3DXMesh * ObjectMesh, D3DXVECTOR3 length, D3DXVECTOR3 ObjPos)
{
	D3DXVECTOR3 vecMin, vecMax, vecMinWorld, vecMaxWorld;
	VOID *ptr = NULL;

	D3DXMatrixIdentity(&m_CBoxWorldMat);

	if (ObjectMesh != NULL)
	{
		// dwVertexNum = Mesh변수->GetNumVertices()
		// dwFvfSize = Mesh변수->GetFVF() -- 다만 이건 VERTEX_PNT:FVF를 해야될듯함
		DWORD dwVertexNum = ObjectMesh->GetNumVertices();
		DWORD dwFvfSize = D3DXGetFVFVertexSize(ObjectMesh->GetFVF());

		// BoundingBox 제작
		ObjectMesh->LockVertexBuffer(0, &ptr);
		D3DXComputeBoundingBox((D3DXVECTOR3 *)ptr, dwVertexNum, dwFvfSize, &vecMin, &vecMax);
		ObjectMesh->UnlockVertexBuffer();
	}
	else
	{
		// 메쉬가 없는 경우
		// 각 변이 length 길이인 정사각형을 만든다 
		vecMin.x = (g_aCubeVertex[0].x * length.x) + ObjPos.x;
		vecMin.y = (g_aCubeVertex[0].y * length.y) + ObjPos.y;
		vecMin.z = (g_aCubeVertex[0].z * length.z) + ObjPos.z;

		vecMax.x = (g_aCubeVertex[6].x * length.x) + ObjPos.x;
		vecMax.y = (g_aCubeVertex[6].y * length.y) + ObjPos.y;
		vecMax.z = (g_aCubeVertex[6].z * length.z) + ObjPos.z;

		D3DXMatrixTranslation(&m_CBoxWorldMat, ObjPos.x, ObjPos.y, ObjPos.z);
	}

	MakeBoundingBox(this, vecMin, vecMax, ObjPos);
}

void CBox::UpdateBoundingBox(D3DXMATRIXA16 &matWorld, float posY)
{
	matWorld._42 += posY;

	D3DXMATRIXA16 mat;
	// 박스의 transform을 가져온다.
	GetBoxTransform(&mat, this);

	mat *= matWorld;

	// 박스의 transform을 바꾼다.
	SetBoxTransform(&mat, this);

	// 박스의 center 좌표도 바꾼다
	center[0] = matWorld._41;
	center[1] = matWorld._42;
	center[2] = matWorld._43;

	m_CBoxWorldMat = matWorld;

	matWorld._42 -= posY;
}

void CBox::RenderBoundingBox()
{
	if (g_DisplayObjMGR->GetBoundingBoxRender() == true)
	{
		g_Device->SetRenderState(D3DRS_LIGHTING, false);

		g_Device->SetTransform(D3DTS_WORLD, &m_CBoxWorldMat);
		g_Device->SetTexture(0, NULL);
		g_Device->SetFVF(VERTEX_PC::FVF);
		g_Device->DrawPrimitiveUP(D3DPT_LINESTRIP, m_vecBoxVertex.size() - 1,
			&m_vecBoxVertex[0], sizeof(VERTEX_PC));

		g_Device->SetRenderState(D3DRS_LIGHTING, true);
	}
}

int BoxBoxIntersectionTest(const CBox &box0, const CBox &box1)
{
    // compute difference of box centers
    // 상자의 center 차이 계산
    // D = C1 - C0
    float D[3] =
        {
            box1.center[0] - box0.center[0],
            box1.center[1] - box0.center[1],
            box1.center[2] - box0.center[2]
        };

    float C[3][3];    // matrix C = A^T B,c_{ij} = Dot(A_i,B_j)
    float absC[3][3]; // |c_{ij}|
    float AD[3];      // Dot(A_i,D)
    float R0, R1, R;  // interval radii and distance between centers
    float R01;        // =R0+R1

    //A0
    C[0][0] = DotProduct(box0.axis[0], box1.axis[0]);
    C[0][1] = DotProduct(box0.axis[0], box1.axis[1]);
    C[0][2] = DotProduct(box0.axis[0], box1.axis[2]);
    AD[0] = DotProduct(box0.axis[0], D);

    absC[0][0] = (float)fc_fabs(C[0][0]);
    absC[0][1] = (float)fc_fabs(C[0][1]);
    absC[0][2] = (float)fc_fabs(C[0][2]);

    R = (float)fc_fabs(AD[0]);
    R1 = box1.extent[0] * absC[0][0] + box1.extent[1] * absC[0][1] + box1.extent[2] * absC[0][2];
    R01 = box0.extent[0] + R1;
    if (R > R01)    
        return 0;

    //A1
    C[1][0] = DotProduct(box0.axis[1], box1.axis[0]);
    C[1][1] = DotProduct(box0.axis[1], box1.axis[1]);
    C[1][2] = DotProduct(box0.axis[1], box1.axis[2]);
    AD[1] = DotProduct(box0.axis[1], D);

    absC[1][0] = (float)fc_fabs(C[1][0]);
    absC[1][1] = (float)fc_fabs(C[1][1]);
    absC[1][2] = (float)fc_fabs(C[1][2]);

    R = (float)fc_fabs(AD[1]);
    R1 = box1.extent[0] * absC[1][0] + box1.extent[1] * absC[1][1] + box1.extent[2] * absC[1][2];
    R01 = box0.extent[1] + R1;
    if (R > R01)   
        return 0;

    //A2
    C[2][0] = DotProduct(box0.axis[2], box1.axis[0]);
    C[2][1] = DotProduct(box0.axis[2], box1.axis[1]);
    C[2][2] = DotProduct(box0.axis[2], box1.axis[2]);
    AD[2] = DotProduct(box0.axis[2], D);

    absC[2][0] = (float)fc_fabs(C[2][0]);
    absC[2][1] = (float)fc_fabs(C[2][1]);
    absC[2][2] = (float)fc_fabs(C[2][2]);
    
    R = (float)fc_fabs(AD[2]);
    R1 = box1.extent[0] * absC[2][0] + box1.extent[1] * absC[2][1] + box1.extent[2] * absC[2][2];
    R01 = box0.extent[2] + R1;
    
    if (R > R01)
        return 0;

    //B0
    R = (float)fc_fabs(DotProduct(box1.axis[0], D));
    R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
    R01 = R0 + box1.extent[0];
    if (R > R01)
        return 0;

    //B1
    R = (float)fc_fabs(DotProduct(box1.axis[1], D));
    R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
    R01 = R0 + box1.extent[1];
    if (R > R01)
        return 0;

    //B2
    R = (float)fc_fabs(DotProduct(box1.axis[2], D));
    R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
    R01 = R0 + box1.extent[2];
    if (R > R01)
        return 0;

    //A0xB0
    R = (float)fc_fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
    R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
    R1 = box1.extent[1] * absC[0][2] + box1.extent[2] * absC[0][1];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

    //A0xB1
    R = (float)fc_fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
    R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
    R1 = box1.extent[0] * absC[0][2] + box1.extent[2] * absC[0][0];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

    //A0xB2
    R = (float)fc_fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
    R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
    R1 = box1.extent[0] * absC[0][1] + box1.extent[1] * absC[0][0];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

    //A1xB0
    R = (float)fc_fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
    R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
    R1 = box1.extent[1] * absC[1][2] + box1.extent[2] * absC[1][1];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

    //A1xB1
    R = (float)fc_fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
    R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
    R1 = box1.extent[0] * absC[1][2] + box1.extent[2] * absC[1][0];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

    //A1xB2
    R = (float)fc_fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
    R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
    R1 = box1.extent[0] * absC[1][1] + box1.extent[1] * absC[1][0];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

    //A2xB0
    R = (float)fc_fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
    R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
    R1 = box1.extent[1] * absC[2][2] + box1.extent[2] * absC[2][1];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

    //A2xB1
    R = (float)fc_fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
    R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
    R1 = box1.extent[0] * absC[2][2] + box1.extent[2] * absC[2][0];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

    //A2xB2
    R = (float)fc_fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
    R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
    R1 = box1.extent[0] * absC[2][1] + box1.extent[1] * absC[2][0];
    R01 = R0 + R1;
    if (R > R01)
        return 0;

	// 충돌!
    return 1;
}
