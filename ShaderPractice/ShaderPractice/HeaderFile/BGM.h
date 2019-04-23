#pragma once
#include "Sound.h"

//BGMのクラス
class BGM :public Sound
{
private:
	//再生中かどうかのフラグ
	bool m_IsPlay;
public:
	//コンストラクタ
	BGM();
	//デストラクタ
	~BGM();
	//再生
	void Play();	
	//ストップ
	void Stop();
	//再生中かどうか
	bool IsPlaying();
};

