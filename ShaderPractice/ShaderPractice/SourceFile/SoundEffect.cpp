#include"stdafx.h"
#include "SoundEffect.h"



SoundEffect::SoundEffect()
{
	m_pSoundBuffer = NULL;
}


SoundEffect::~SoundEffect()
{
	Stop();
	m_pSoundBuffer->Release();
	m_pSoundBuffer = NULL;
}
//Ä¶
void SoundEffect::Play()
{
	if (m_pSoundBuffer != NULL)
		m_pSoundBuffer->Play(0, 0, 0);
}
//’âŽ~
void SoundEffect::Stop()
{
	if (m_pSoundBuffer != NULL)
		m_pSoundBuffer->Stop();
}
