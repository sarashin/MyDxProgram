#include"stdafx.h"
#include "Sound.h"



Sound::Sound()
{
	
}


Sound::~Sound()
{	
	
}
//サウンドをセット
void Sound::SetSound(IDirectSoundBuffer8* pSound)
{
	m_pSoundBuffer = pSound;
}