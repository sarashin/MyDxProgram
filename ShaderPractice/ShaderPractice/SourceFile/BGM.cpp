#include"stdafx.h"
#include "BGM.h"

//コンストラクタ
BGM::BGM()
{
	m_pSoundBuffer = NULL;
}

//デストラクタ
BGM::~BGM()
{
	Stop();
	m_pSoundBuffer = NULL;
}

//再生
void BGM::Play()
{
	if (m_pSoundBuffer != NULL)
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);

	m_IsPlay = true;
}

//中止
void BGM::Stop()
{
	if (m_pSoundBuffer != NULL)
		m_pSoundBuffer->Stop();

	m_IsPlay = false;
}

//再生中かどうか
bool BGM::IsPlaying()
{
	return m_IsPlay;
}