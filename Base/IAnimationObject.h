#pragma once
#include "../Loader/DSkinnedMesh.h"
#include "../Loader/AllocateHierarchy.h"

// �ִϸ��̼ǿ�����Ʈ -> ���λ� A
// x file�� �����ϴ� �ִϸ��̼��� �ʿ��� ������Ʈ�� ��ü��
// Player���� ��ü�� IAnimationObject�� ��ӹ����� �ȴ�
class IAnimationObject : public DSkinnedMesh
{
protected:

public:
	IAnimationObject();
	~IAnimationObject();

	// x file�� ��ο� �̸��� �����ϴ� �޼���
	// DSkinnedMesh::init()���� ���� ����Ǿ�� ��
	// ��� ���� : m_Pplayer->SetXfilePath("Resource/Player/", "Player.X");
	void SetXfilePath(CString filePath, CString fileName);
	
	// ���� �ִϸ��̼��� �ε����� �����ϴ� �޼���
	// �ϴ��� PLAYER Enum ����� ���� ���ο� Enum�� �����ؼ� ����Ұ�
	// ��� ���� : m_Pplayer->SetNowAnimation(PLAYER_AM::PLAYER_STAND);
	void SetNowAnimation(int index) { m_EanimIndex = index; }

	// ��ӹ��� ��ü�� ���� �ڽ��� ��ü�� 
	// DSkinnedMesh::init()
	// DSkinnedMesh::Update()
	// DSkinnedMesh::Render()
	// �Լ��� init/Update/Render���� ȣ����Ѿ� �Ѵ�

	// map<Bone�� �̸�, Bone Frame ����>
	map<LPCSTR, LPD3DXFRAME> GetBonelist() { return m_vecBonelist; }		// Bonelist�� ��ȯ�ϴ� �޼���
	D3DXMATRIXA16 FindBoneTM(LPCSTR str)
	{
		for (auto p : m_vecBonelist)
		{
			if (p.second->Name != NULL && strcmp(p.second->Name, str) == 0)
			{
				FRAME_EX* pFrameEx = (FRAME_EX*)(p.second);
				return pFrameEx->CombinedTM;
			}
		}
	}	// Ư�� Bone�� ��ȯ�ϴ� �޼���

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

	// ���� ���� �ȵ�!
	PLAYER_AIM,				// ������
	PLAYER_RELOAD,			// ����

	PLAYER_CROUCH,			// �ɱ�
	PLAYER_CROUCH_WALK,		// �ɾƼ� �ȱ�
	PLAYER_CROUCH_AIM,		// �ɾƼ� ������
	PLAYER_CROUCH_RELOAD,	// �ɾƼ� ����
	PLAYER_CROUCH_HEAL		// �ɾƼ� �ش밨��
};