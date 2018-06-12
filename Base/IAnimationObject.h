#pragma once
#include "../Loader/DSkinnedMesh.h"
#include "../Loader/AllocateHierarchy.h"

// 애니메이션오브젝트 -> 접두사 A
// x file로 동작하는 애니매이션이 필요한 오브젝트의 객체형
// Player등의 객체는 IAnimationObject를 상속받으면 된다
class IAnimationObject : public DSkinnedMesh
{
protected:

public:
	IAnimationObject();
	~IAnimationObject();

	// x file의 경로와 이름을 설정하는 메서드
	// DSkinnedMesh::init()보다 먼저 실행되어야 함
	// 사용 예시 : m_Pplayer->SetXfilePath("Resource/Player/", "Player.X");
	void SetXfilePath(CString filePath, CString fileName);
	
	// 현재 애니매이션의 인덱스를 설정하는 메서드
	// 하단의 PLAYER Enum 방식을 보고 새로운 Enum을 생성해서 사용할것
	// 사용 예시 : m_Pplayer->SetNowAnimation(PLAYER_AM::PLAYER_STAND);
	void SetNowAnimation(int index) { m_EanimIndex = index; }

	// 상속받은 객체는 각각 자신의 객체의 
	// DSkinnedMesh::init()
	// DSkinnedMesh::Update()
	// DSkinnedMesh::Render()
	// 함수를 init/Update/Render에서 호출시켜야 한다

	// map<Bone의 이름, Bone Frame 변수>
	map<LPCSTR, LPD3DXFRAME> GetBonelist() { return m_vecBonelist; }		// Bonelist를 반환하는 메서드
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
	}	// 특정 Bone을 반환하는 메서드

	// Bonelist를 출력하는 메서드
	void PrintBoneList();
};


// x file의 경우 파일의 맨 아래에 있는 AnimationSet이 0번째, 맨 아래의 위 AnimationSet 1번째...
enum PLAYER_AM
{
	PLAYER_STAND,			// 서있기
	PLAYER_MOVE,			// 일반적인 이동
	PLAYER_WALK,			// 천천히 걷기
	PLAYER_RUN,				// 달리기
	PLAYER_HEAL,			// 붕대감기

	// 아직 구현 안됨!
	PLAYER_AIM,				// 정조준
	PLAYER_RELOAD,			// 장전

	PLAYER_CROUCH,			// 앉기
	PLAYER_CROUCH_WALK,		// 앉아서 걷기
	PLAYER_CROUCH_AIM,		// 앉아서 정조준
	PLAYER_CROUCH_RELOAD,	// 앉아서 장전
	PLAYER_CROUCH_HEAL		// 앉아서 붕대감기
};