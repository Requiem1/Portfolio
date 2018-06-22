#pragma once
// Created by zoi@3map.snu.ac.kr
// for SNU CG DirectX lecture
// 출저 : http://www.gingaminga.com/Data/Note/oriented_bounding_boxes/

class CBox
{
public:
	vector<VERTEX_PC> vecBoxVertex;

    float center[3];
    float axis[3][3];     // transformation matrix
    float extent[3];      // half full size-eqivalent to radius not diameter
    float translation[3]; // translation vector

    void GetTransform(float t[16])
    {
        t[0] = axis[0][0];      // _11
        t[1] = axis[0][1];      // _12
        t[2] = axis[0][2];      // _13
        t[3] = ((float)0.0);    // _14

        t[4] = axis[1][0];      // _21
        t[5] = axis[1][1];      // _22
        t[6] = axis[1][2];      // _23
        t[7] = ((float)0.0);    // _24

        t[8] = axis[2][0];      // _31
        t[9] = axis[2][1];      // _32
        t[10] = axis[2][2];     // _33
        t[11] = ((float)0.0);   // _34

        t[12] = translation[0]; // _41
        t[13] = translation[1]; // _42
        t[14] = translation[2]; // _43
        t[15] = ((float)1.0);   // _44
    }

	// OBB 바운싱 박스 초기화 & 제작
	void initBoundingBox(ID3DXMesh * ObjectMesh, D3DXVECTOR3 length = D3DXVECTOR3(1, 1, 1), D3DXVECTOR3 ObjPos = D3DXVECTOR3(0,0,0));
	void MakeBoundingBox(CBox *pBox, const D3DXVECTOR3 &vecMin, const D3DXVECTOR3 &vecMax);

	// 바운싱박스의 update/render 함수
	void UpdateBoundingBox(D3DXMATRIXA16 &matWorld);
	void RenderBoundingBox();
};

int BoxBoxIntersectionTest(const CBox &box0, const CBox &box1);

inline float DotProduct(const float v0[3], const float v1[3])
{   
    // = v0 dot v1
    return (v0[0] * v1[0]) + (v0[1] * v1[1]) + (v0[2] * v1[2]);
}

inline D3DXMATRIXA16 *GetBoxTransform(D3DXMATRIXA16 *pMat, CBox *pBox)
{
	int i, j;
	float fMat[16];
	pBox->GetTransform(fMat);

	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			(*pMat)(i, j) = fMat[i * 4 + j];

	return pMat;
}

inline void SetBoxTransform(const D3DXMATRIXA16 *pMat, CBox *pBox)
{
	int i, j;
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 3; ++j)
			pBox->axis[i][j] = (*pMat)(i, j);
		pBox->translation[i] = (*pMat)(3, i);
	}
}


#define fc_fabs fabsf
