#pragma once
#include "../Loader/DSkinnedMesh.h"
#include "../Loader/AllocateHierarchy.h"

// 애니메이션오브젝트 -> 접두사 A
// x file로 동작하는 애니매이션이 필요한 오브젝트의 객체형
// Player등의 객체는 IAnimationObject를 상속받으면 된다
class IAnimationObject : public DSkinnedMesh
{
protected:
	D3DXVECTOR3	m_ScaleXYZ;	// x파일 크기 설정 매트릭스

	//D3DXMATRIXA16 matS;
	//D3DXMatrixScaling(&matS, m_ScaleXYZ.x, m_ScaleXYZ.y, m_ScaleXYZ.z);
	// X파일의 크기 줄여야 할 시 적용!
	void SetScale(float _S) { m_ScaleXYZ = D3DXVECTOR3(_S, _S, _S); }	

public:
	IAnimationObject();
	~IAnimationObject();

	// x file의 경로와 이름을 설정하는 메서드
	// DSkinnedMesh::init()보다 먼저 실행되어야 함
	// 사용 예시 : m_Pplayer->SetXfilePath("Resource/Player/", "Player.X");
	// 만일 크기 변경이 필요할 경우 ScaleVector에 크기를 넣으면 된다 -> SetXfilePath("Resource/P/", "p.X", D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	void SetXfile(CString filePath, CString fileName, float ScaleXYZ = 1.0f);
	
	// 애니매이션의 인덱스를 설정하는 메서드
	// 하단의 PLAYER Enum 방식을 보고 새로운 Enum을 생성해서 사용할것
	// 사용 예시 : m_Pplayer->SetNowAnimation(PLAYER_STAND), m_Pplayer->SetNowAnimation(PLAYER_HEAL, false)
	void SetNowAnimation(int index, bool isLoop = true)			{ m_EanimIndex_Down = index; m_BAniLoop_Down = isLoop; }

	// 하체/상체 애니매이션의 인덱스를 설정하는 메서드
	// 사실 루트가 하체이므로 Down을 따로 만들 필요는 없지만.. Up이 있는데 Down이 없으면 헷갈리니까...
	void SetNowAnimation_Down(int index, bool isLoop = true)	{ m_EanimIndex_Down = index; m_BAniLoop_Down = isLoop; }
	void SetNowAnimation_Up(int index, bool isLoop = true)		{ m_EanimIndex_Up = index; m_BAniLoop_Up = isLoop;	}


	// 상속받은 객체는 각각 자신의 객체의 
	// DSkinnedMesh::init()
	// DSkinnedMesh::Update()
	// DSkinnedMesh::Render()
	// 함수를 init/Update/Render에서 호출시켜야 한다


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

	// 하단은 아직 구현 안됨!
	PLAYER_AIM,				// 정조준
	PLAYER_RELOAD,			// 장전

	PLAYER_CROUCH,			// 앉기
	PLAYER_CROUCH_WALK,		// 앉아서 걷기
	PLAYER_CROUCH_AIM,		// 앉아서 정조준
	PLAYER_CROUCH_RELOAD,	// 앉아서 장전
	PLAYER_CROUCH_HEAL		// 앉아서 붕대감기
};

enum ZOMBIE_AM
{
	ZOMBIE_IDLE,			// 서있기
	ZOMBIE_SCREAM,			// 소리치기
	ZOMBIE_RUN,				// 달리기
	ZOMBIE_ATTACK,			// 한손 공격
	ZOMBIE_DEAD,			// 사망 모션
	ZOMBIE_CORPSE			// 시체상태
};