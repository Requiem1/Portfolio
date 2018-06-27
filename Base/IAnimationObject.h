#pragma once
#include "../Loader/DSkinnedMesh.h"
#include "../Loader/AllocateHierarchy.h"

// �ִϸ��̼ǿ�����Ʈ -> ���λ� A
// x file�� �����ϴ� �ִϸ��̼��� �ʿ��� ������Ʈ�� ��ü��
// Player���� ��ü�� IAnimationObject�� ��ӹ����� �ȴ�
class IAnimationObject : public DSkinnedMesh
{
protected:
	D3DXVECTOR3	m_ScaleXYZ;	// x���� ũ�� ���� ��Ʈ����

	//D3DXMATRIXA16 matS;
	//D3DXMatrixScaling(&matS, m_ScaleXYZ.x, m_ScaleXYZ.y, m_ScaleXYZ.z);
	// X������ ũ�� �ٿ��� �� �� ����!
	void SetScale(float _S) { m_ScaleXYZ = D3DXVECTOR3(_S, _S, _S); }	

public:
	IAnimationObject();
	~IAnimationObject();

	// x file�� ��ο� �̸��� �����ϴ� �޼���
	// DSkinnedMesh::init()���� ���� ����Ǿ�� ��
	// ��� ���� : m_Pplayer->SetXfilePath("Resource/Player/", "Player.X");
	// ���� ũ�� ������ �ʿ��� ��� ScaleVector�� ũ�⸦ ������ �ȴ� -> SetXfilePath("Resource/P/", "p.X", D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	void SetXfile(CString filePath, CString fileName, float ScaleXYZ = 1.0f);
	
	// �ִϸ��̼��� �ε����� �����ϴ� �޼���
	// �ϴ��� PLAYER Enum ����� ���� ���ο� Enum�� �����ؼ� ����Ұ�
	// ��� ���� : m_Pplayer->SetNowAnimation(PLAYER_STAND), m_Pplayer->SetNowAnimation(PLAYER_HEAL, false)
	void SetNowAnimation(int index, bool isLoop = true)			{ m_EanimIndex_Down = index; m_BAniLoop_Down = isLoop; }

	// ��ü/��ü �ִϸ��̼��� �ε����� �����ϴ� �޼���
	// ��� ��Ʈ�� ��ü�̹Ƿ� Down�� ���� ���� �ʿ�� ������.. Up�� �ִµ� Down�� ������ �򰥸��ϱ�...
	void SetNowAnimation_Down(int index, bool isLoop = true)	{ m_EanimIndex_Down = index; m_BAniLoop_Down = isLoop; }
	void SetNowAnimation_Up(int index, bool isLoop = true)		{ m_EanimIndex_Up = index; m_BAniLoop_Up = isLoop;	}


	// ��ӹ��� ��ü�� ���� �ڽ��� ��ü�� 
	// DSkinnedMesh::init()
	// DSkinnedMesh::Update()
	// DSkinnedMesh::Render()
	// �Լ��� init/Update/Render���� ȣ����Ѿ� �Ѵ�


	// Bonelist�� ����ϴ� �޼���
	void PrintBoneList();
};


// x file�� ��� ������ �� �Ʒ��� �ִ� AnimationSet�� 0��°, �� �Ʒ��� �� AnimationSet 1��°...
enum PLAYER_AM
{
	PLAYER_STAND,			// ���ֱ�
	PLAYER_MOVE,			// �Ϲ����� �̵�
	PLAYER_WALK,			// õõ�� �ȱ�
	PLAYER_RUN,				// �޸���
	PLAYER_HEAL,			// �ش밨��

	// �ϴ��� ���� ���� �ȵ�!
	PLAYER_AIM,				// ������
	PLAYER_RELOAD,			// ����

	PLAYER_CROUCH,			// �ɱ�
	PLAYER_CROUCH_WALK,		// �ɾƼ� �ȱ�
	PLAYER_CROUCH_AIM,		// �ɾƼ� ������
	PLAYER_CROUCH_RELOAD,	// �ɾƼ� ����
	PLAYER_CROUCH_HEAL		// �ɾƼ� �ش밨��
};

enum ZOMBIE_AM
{
	ZOMBIE_IDLE,			// ���ֱ�
	ZOMBIE_SCREAM,			// �Ҹ�ġ��
	ZOMBIE_RUN,				// �޸���
	ZOMBIE_ATTACK,			// �Ѽ� ����
	ZOMBIE_DEAD,			// ��� ���
	ZOMBIE_CORPSE			// ��ü����
};