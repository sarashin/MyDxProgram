#include"stdafx.h"
#include "Sound.h"



Sound::Sound()
{
	
}


Sound::~Sound()
{	
	
}
//�T�E���h���Z�b�g
void Sound::SetSound(IDirectSoundBuffer8* pSound)
{
	m_pSoundBuffer = pSound;
}