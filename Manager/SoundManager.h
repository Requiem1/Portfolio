#pragma once

// FMOD ���̺귯�� ȣ���
#include "../inc/fmod.hpp"
#pragma comment (lib, "fmodex_vc.lib")
using namespace FMOD;

// ���� ����
#define EXTRACHANNELBUFFER 15
#define SOUNDBUFFER 43

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

#define BGM_INDEX 0

#define g_SoundMGR soundManager::GetInstance()

// 3d���� ���� : https://www.fmod.com/resources/documentation-api?page=content/generated/overview/3dsound.html#/
// FMOD API ��ġ�ÿ� ������ �������ϵ� ���� ������

// https://www.fmod.com/resources/documentation-api?page=content/generated/overview/3dsound.html#/
struct Listener
{
	D3DXVECTOR3 * listener_pos;		// �������� ��ġ
	D3DXVECTOR3 * listener_vel;		// �����ʿ��� ���� �Ҹ��� �ӵ� -> �⺻���� 0�� ����� ���̹Ƿ� �������� ����
	D3DXVECTOR3 * listener_forward;	// �������� ������
	D3DXVECTOR3 * listener_up;		// �������� ������
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



	bool						m_BisBGMOn;			// BGM�� �����ִ��� Ȯ��

	Listener					m_Listeners[4];		// �Ҹ��� ��� �÷��̾� ����ü. 4���� �÷��̾ ���ߵǹǷ�!
	map<string, D3DXVECTOR3*>	m_vecSpeakerPos;	// �Ҹ��� ��µǴ� ��ġ���� ����

	FMOD_VECTOR					m_BaseVel;			// �⺻ �ӵ�(velocity) = { 0,0,0 };
	

public:
	// ���� ��� ����
	// 	g_SoundMGR->AddSound("Checkpoint_fire", "ruger77-fire.wav", false, false, &m_pos);
	//
	//	if (GetAsyncKeyState('7') & 0x0001)
	//		g_SoundMGR->Play("Checkpoint_fire");


	// Application���� ���۽��Ѿ���
	void Init();
	void Update();

	// ���� �߰� �Լ�
	// AddSound(�ҷ��ö� ����� Ű �̸�, ���� ���ϸ�, BGM����, �ݺ��Ǵ� ��������, ���� ��ġ!!)
	// ex) AddSound("ShotGun_Fire", "spas12-fire.wav", false, false, &m_pos);
	void AddSound(string keyName, string soundName, bool isBgm = false, bool isLoop = false, D3DXVECTOR3* pos = NULL);

	// ���� ��� �Լ�
	// Play(�ҷ��ö� ����� Ű �̸�, BGM����, ����) -> ���� : 0.0 ~ 1.0(������ �ִ� 255)
	// ex) Play("ShotGun_Fire", flase);
	void Play(string keyName, bool isBgm = false, float volume = 1.0f);				

	// ���� ���� �Լ�
	// Stop(BGM����, ������ Ű �̸�(BGM�� �ƴҶ� �Է��ϸ� �ȴ�) )
	void Stop(bool isBgm, string keyName = "");


	// �Ͻ�����/�����
	// �Ƹ� ���� ������...
	void Pause(bool isBgm, string keyName = "");
	void Resume(bool isBgm, string keyName = "");

	// ���� keyName�� ������ ��������� Ȯ��
	bool IsPlaySound(string keyName, bool isBgm);
	bool IsPauseSound(string keyName, bool isBgm);

	// D3DXVECTOR3�� FMOD_VECTOR�� ��ȯ�����ִ� �Լ�
	FMOD_VECTOR GetFVec(D3DXVECTOR3* vec)
	{
		FMOD_VECTOR Fvec = { vec->x, vec->y, vec->z };
		return Fvec;
	}

	// Listener�� ����
	void SetListener(int index, D3DXVECTOR3* pos, D3DXVECTOR3* forward, D3DXVECTOR3* up)
	{
		m_Listeners[index].listener_pos = pos;
		m_Listeners[index].listener_vel = 0;			// ���÷� ȿ���� ���Ҷ��� ����ϹǷ�.. �⺻���� 0����
		m_Listeners[index].listener_forward = forward;	
		m_Listeners[index].listener_up = up;


		// �÷��̾�1�� ������ init
		// listener_vel, �� velocity�� �⺻���� 0�� �����Ѵ�
		pSystem->set3DListenerAttributes(
			index,									
			&GetFVec(m_Listeners[0].listener_pos),
			NULL,									//&GetFVec(*m_Listeners[0].listener_vel),
			&GetFVec(m_Listeners[0].listener_forward),
			&GetFVec(m_Listeners[0].listener_up)
		);
	}
};

