#pragma once

// FMOD 라이브러리 호출용
#include "../inc/fmod.hpp"
#pragma comment (lib, "fmodex_vc.lib")
using namespace FMOD;

// 사운드 버퍼
#define EXTRACHANNELBUFFER 15
#define SOUNDBUFFER 43

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

#define BGM_INDEX 0

#define g_SoundMGR soundManager::GetInstance()

// 3d사운드 출저 : https://www.fmod.com/resources/documentation-api?page=content/generated/overview/3dsound.html#/
// FMOD API 설치시에 나오는 예제파일들 또한 참고함

// https://www.fmod.com/resources/documentation-api?page=content/generated/overview/3dsound.html#/
struct Listener
{
	D3DXVECTOR3 * listener_pos;		// 리스너의 위치
	D3DXVECTOR3 * listener_vel;		// 리스너에게 오는 소리의 속도 -> 기본값인 0을 사용할 것이므로 조작하지 않음
	D3DXVECTOR3 * listener_forward;	// 리스너의 포워드
	D3DXVECTOR3 * listener_up;		// 리스너의 업벡터
};

class soundManager
{
	SINGLETON(soundManager)

private:
	typedef map<string, Sound*> mapSounds;
	typedef map<string, Channel*> arrChannels;

	typedef map<string, Sound*>::iterator mapSoundsIter;
	typedef map<string, Channel*>::iterator arrChannelsIter;

	System* pSystem;			
	Sound** ppSound;			
	Channel** ppChannel;		
	mapSounds TotalSounds;		



	bool						m_BisBGMOn;			// BGM이 켜져있는지 확인

	Listener					m_Listeners[4];		// 소리를 듣는 플레이어 구조체. 4명의 플레이어가 들어야되므로!
	map<string, D3DXVECTOR3*>	m_vecSpeakerPos;	// 소리가 출력되는 위치들의 집합

	FMOD_VECTOR					m_BaseVel;			// 기본 속도(velocity) = { 0,0,0 };
	

public:
	// 실제 사용 예시
	// 	g_SoundMGR->AddSound("Checkpoint_fire", "ruger77-fire.wav", false, false, &m_pos);
	//
	//	if (GetAsyncKeyState('7') & 0x0001)
	//		g_SoundMGR->Play("Checkpoint_fire");


	// Application에서 동작시켜야함
	void Init();
	void Update();

	// 사운드 추가 함수
	// AddSound(불러올때 사용할 키 이름, 사운드 파일명, BGM인지, 반복되는 음악인지, 현재 위치!!)
	// ex) AddSound("ShotGun_Fire", "spas12-fire.wav", false, false, &m_pos);
	void AddSound(string keyName, string soundName, bool isBgm = false, bool isLoop = false, D3DXVECTOR3* pos = NULL);

	// 사운드 재생 함수
	// Play(불러올때 사용할 키 이름, BGM인지, 볼륨) -> 볼륨 : 0.0 ~ 1.0(원래는 최대 255)
	// ex) Play("ShotGun_Fire", flase);
	void Play(string keyName, bool isBgm = false, float volume = 1.0f);				

	// 사운드 종료 함수
	// Stop(BGM인지, 종료할 키 이름(BGM이 아닐때 입력하면 된다) )
	void Stop(bool isBgm, string keyName = "");


	// 일시정지/재시작
	// 아마 쓸일 없을듯...
	void Pause(bool isBgm, string keyName = "");
	void Resume(bool isBgm, string keyName = "");

	// 현재 keyName의 음악이 재생중인지 확인
	bool IsPlaySound(string keyName, bool isBgm);
	bool IsPauseSound(string keyName, bool isBgm);

	// D3DXVECTOR3를 FMOD_VECTOR로 변환시켜주는 함수
	FMOD_VECTOR GetFVec(D3DXVECTOR3* vec)
	{
		FMOD_VECTOR Fvec = { vec->x, vec->y, vec->z };
		return Fvec;
	}

	// Listener를 설정
	void SetListener(int index, D3DXVECTOR3* pos, D3DXVECTOR3* forward, D3DXVECTOR3* up)
	{
		m_Listeners[index].listener_pos = pos;
		m_Listeners[index].listener_vel = 0;			// 도플러 효과를 원할때만 사용하므로.. 기본값인 0으로
		m_Listeners[index].listener_forward = forward;	
		m_Listeners[index].listener_up = up;


		// 플레이어1의 리스너 init
		// listener_vel, 즉 velocity는 기본값인 0을 설정한다
		pSystem->set3DListenerAttributes(
			index,									
			&GetFVec(m_Listeners[0].listener_pos),
			NULL,									//&GetFVec(*m_Listeners[0].listener_vel),
			&GetFVec(m_Listeners[0].listener_forward),
			&GetFVec(m_Listeners[0].listener_up)
		);
	}
};

