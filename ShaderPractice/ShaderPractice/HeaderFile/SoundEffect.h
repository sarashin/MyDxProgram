#pragma once
#include "Sound.h"
class SoundEffect :public Sound
{
public:
	//�R���X�g���N�^
	SoundEffect();
	//�f�X�g���N�^
	~SoundEffect();
	//�Đ�
	void Play();
	//���~
	void Stop();
};

