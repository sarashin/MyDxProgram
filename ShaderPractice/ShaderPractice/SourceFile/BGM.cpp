#include"stdafx.h"
#include "BGM.h"

//�R���X�g���N�^
BGM::BGM()
{
	m_pSoundBuffer = NULL;
}

//�f�X�g���N�^
BGM::~BGM()
{
	Stop();
	m_pSoundBuffer = NULL;
}

//�Đ�
void BGM::Play()
{
	if (m_pSoundBuffer != NULL)
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);

	m_IsPlay = true;
}

//���~
void BGM::Stop()
{
	if (m_pSoundBuffer != NULL)
		m_pSoundBuffer->Stop();

	m_IsPlay = false;
}

//�Đ������ǂ���
bool BGM::IsPlaying()
{
	return m_IsPlay;
}