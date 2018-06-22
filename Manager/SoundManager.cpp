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

	//ä�ΰ� ����� �����Ҵ�!!
	ppSound = new Sound*[TOTALSOUNDBUFFER];
	ppChannel = new Channel*[TOTALSOUNDBUFFER];

	memset(ppSound, 0, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	memset(ppChannel, 0, sizeof(Channel*) * (TOTALSOUNDBUFFER));

	g_SoundMGR->AddSound("Checkpoint_BGM", "DarkKnightOST_WhySoSerious.mp3", true, false);
}

void soundManager::Update()
{
	// ���� ���۽� 4���÷��̸� ����ؼ�
	// Player�� ���ڸ�ŭ Listener�� ��ġ�� Update�Ѵ�
	for (int i = 0; i < g_PlayerMGR->GetPlayer().size(); i++)
	{
		pSystem->set3DListenerAttributes(
			i,										// �÷��̾� index
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
		// createStream -> BGM_INDEX�� ä�ο� �����ȴ� 
		if (isBgm)
		{
			soundName = "Resource/Sound/BGM/" + soundName;
			pSystem->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &ppSound[TotalSounds.size()]);
		}
		else
		{
			soundName = "Resource/Sound/SE/" + soundName;
			pSystem->createSound(soundName.c_str(), FMOD_LOOP_NORMAL | FMOD_3D, 0, &ppSound[TotalSounds.size()]);

			// set3DMinMaxDistance(min, max) : 3d ���� min max ����
			// �Ҹ����� �鸮�� �Ÿ��� �����ϰ� �ʹٸ� ������ min max ���� ��ġ�� �ȴ�
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

			// set3DMinMaxDistance(min, max) : 3d ���� min max ����
			// �Ҹ����� �鸮�� �Ÿ��� �����ϰ� �ʹٸ� ������ min max ���� ��ġ�� �ȴ�
			ppSound[TotalSounds.size()]->set3DMinMaxDistance(15.0f, 10000.0f);
			m_vecSpeakerPos[keyName] = pos;
		}
	}

	TotalSounds.insert( make_pair(keyName, ppSound[TotalSounds.size()]) );
	printf("AddSound ȣ��!! ���� TotalSound ũ�� : %d\n", TotalSounds.size());
	cout << "key : " << keyName << " / File : " << soundName << endl;
}

void soundManager::Play(string keyName, bool isBgm, float volume)
{
	mapSoundsIter iter;

	int count = 1;
	cout << "���� Play : " << keyName << endl;

	// On�� ���ǵ��� ��� Play!
	for (iter = TotalSounds.begin(); iter != TotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			if (!isBgm)
			{
				// ���� �Ҹ��� ���ڱ� �ȳ����� ��Ȳ�� ����ٸ� FMOD_CHANNEL_FREE ��� FMOD_CHANNEL_REUSE�� ����غ��� �ٶ�
				pSystem->playSound(FMOD_CHANNEL_FREE, iter->second, false, &ppChannel[count]);
				
				// set3DAttributes(�Ҹ��� ���� ��ġ, �ӵ�->�����ص���(0,0,0))
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
