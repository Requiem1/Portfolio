#include "../stdafx.h"
#include "soundManager.h"

soundManager::soundManager()
{
	m_BisBGMOn = true;
	m_BaseVel = { 0,0,0 };
}


soundManager::~soundManager()
{
	SAFE_DELETE_ARRAY(ppSound);
	SAFE_DELETE_ARRAY(ppChannel);
}

void soundManager::Init()
{
	System_Create(&pSystem);

	pSystem->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//채널과 사운드는 동적할당!!
	ppSound = new Sound*[TOTALSOUNDBUFFER];
	ppChannel = new Channel*[TOTALSOUNDBUFFER];

	memset(ppSound, 0, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	memset(ppChannel, 0, sizeof(Channel*) * (TOTALSOUNDBUFFER));

	g_SoundMGR->AddSound("Checkpoint_BGM", "DarkKnightOST_WhySoSerious.mp3", true, false);
}

void soundManager::Update()
{
	// 서버 동작시 4인플레이를 대비해서
	// Player의 숫자만큼 Listener의 위치를 Update한다
	for (int i = 0; i < g_PlayerMGR->GetPlayer().size(); i++)
	{
		pSystem->set3DListenerAttributes(
			i,										// 플레이어 index
			&GetFVec(m_Listeners[i].listener_pos),
			NULL,									//&GetFVec(*m_Listeners[0].listener_vel),
			&GetFVec(m_Listeners[i].listener_forward),
			&GetFVec(m_Listeners[i].listener_up)
		);
	}

	pSystem->update();

	if (!m_BisBGMOn)	ppChannel[BGM_INDEX]->setVolume(0);
	else				ppChannel[BGM_INDEX]->setVolume(1);
}

void soundManager::AddSound(string keyName, string soundName, bool isBgm, bool isLoop, D3DXVECTOR3* pos)
{
	if (isLoop)
	{
		// createStream -> BGM_INDEX의 채널에 생성된다 
		if (isBgm)
		{
			soundName = "Resource/Sound/BGM/" + soundName;
			pSystem->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &ppSound[TotalSounds.size()]);
		}
		else
		{
			soundName = "Resource/Sound/SE/" + soundName;
			pSystem->createSound(soundName.c_str(), FMOD_LOOP_NORMAL | FMOD_3D, 0, &ppSound[TotalSounds.size()]);

			// set3DMinMaxDistance(min, max) : 3d 사운드 min max 조절
			// 소리값이 들리는 거리를 조절하고 싶다면 여기의 min max 값을 고치면 된다
			ppSound[TotalSounds.size()]->set3DMinMaxDistance(15.0f, 5000.0f);
			m_vecSpeakerPos[keyName] = pos;
		}
	}
	else
	{
		if (isBgm)
		{
			soundName = "Resource/Sound/BGM/" + soundName;
			pSystem->createStream(soundName.c_str(), FMOD_DEFAULT, 0, &ppSound[TotalSounds.size()]);
		}
		else
		{
			soundName = "Resource/Sound/SE/" + soundName;
			pSystem->createSound(soundName.c_str(), FMOD_3D, 0, &ppSound[TotalSounds.size()]);

			// set3DMinMaxDistance(min, max) : 3d 사운드 min max 조절
			// 소리값이 들리는 거리를 조절하고 싶다면 여기의 min max 값을 고치면 된다
			ppSound[TotalSounds.size()]->set3DMinMaxDistance(15.0f, 10000.0f);
			m_vecSpeakerPos[keyName] = pos;
		}
	}

	TotalSounds.insert( make_pair(keyName, ppSound[TotalSounds.size()]) );
	printf("AddSound 호출!! 현재 TotalSound 크기 : %d\n", TotalSounds.size());
	cout << "key : " << keyName << " / File : " << soundName << endl;
}

void soundManager::Play(string keyName, bool isBgm, float volume)
{
	mapSoundsIter iter;

	int count = 1;
	cout << "음악 Play : " << keyName << endl;

	// On된 음악들을 모두 Play!
	for (iter = TotalSounds.begin(); iter != TotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			if (!isBgm)
			{
				// 만일 소리가 갑자기 안나오는 상황이 생긴다면 FMOD_CHANNEL_FREE 대신 FMOD_CHANNEL_REUSE를 사용해보기 바람
				pSystem->playSound(FMOD_CHANNEL_FREE, iter->second, false, &ppChannel[count]);
				
				// set3DAttributes(소리가 나는 위치, 속도->계산안해도됨(0,0,0))
				ppChannel[count]->set3DAttributes(&GetFVec(m_vecSpeakerPos[keyName]), &m_BaseVel);
				ppChannel[count - 1]->setVolume(volume);
			}

			if (isBgm)
			{
				bool isPlayingBool;
				ppChannel[BGM_INDEX]->isPlaying(&isPlayingBool);

				if (isPlayingBool)
					return;

				pSystem->playSound(FMOD_CHANNEL_REUSE, iter->second, false, &ppChannel[BGM_INDEX]);
				ppChannel[BGM_INDEX]->setVolume(volume);
			}
			break;
		}
	}
}

void soundManager::Stop(bool isBgm, string keyName)
{
	if (isBgm)
	{
		ppChannel[BGM_INDEX]->stop();
		return;
	}

	mapSoundsIter iter = TotalSounds.begin();

	int count = 1;

	for (iter; iter != TotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
			ppChannel[count]->stop();
	}
}

void soundManager::Pause(bool isBgm, string keyName)
{
	if (isBgm)
	{
		m_BisBGMOn = false;
		return;
	}

	mapSoundsIter iter = TotalSounds.begin();

	int count = 0;

	for (iter; iter != TotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			ppChannel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::Resume(bool isBgm, string keyName)
{
	if (isBgm)
	{
		ppChannel[BGM_INDEX]->setPosition(BGM_INDEX, FMOD_TIMEUNIT_MS);
		return;
	}

	mapSoundsIter iter = TotalSounds.begin();

	int count = 0;

	for (iter; iter != TotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			ppChannel[count]->setPaused(false);
			break;
		}
	}
}

bool soundManager::IsPlaySound(string keyName, bool isBgm)
{
	bool isPlay;
	mapSoundsIter iter = TotalSounds.begin();

	int count = 1;

	for (iter; iter != TotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			if (!isBgm)	ppChannel[count]->isPlaying(&isPlay);
			if (isBgm)	ppChannel[BGM_INDEX]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool soundManager::IsPauseSound(string keyName, bool isBgm)
{
	bool isPause;
	mapSoundsIter iter = TotalSounds.begin();

	int count = 1;

	for (iter; iter != TotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			if (!isBgm)	ppChannel[count]->getPaused(&isPause);
			if (isBgm)	ppChannel[0]->getPaused(&isPause);
			break;
		}
	}
	return false;
}
