#pragma once
#include "Sound.h"
class SoundEffect :public Sound
{
public:
	//コンストラクタ
	SoundEffect();
	//デストラクタ
	~SoundEffect();
	//再生
	void Play();
	//中止
	void Stop();
};

