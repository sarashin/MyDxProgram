#pragma once
#include "Sound.h"

//BGM�̃N���X
class BGM :public Sound
{
private:
	//�Đ������ǂ����̃t���O
	bool m_IsPlay;
public:
	//�R���X�g���N�^
	BGM();
	//�f�X�g���N�^
	~BGM();
	//�Đ�
	void Play();	
	//�X�g�b�v
	void Stop();
	//�Đ������ǂ���
	bool IsPlaying();
};

