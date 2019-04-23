#pragma once
#include "Sound.h"

//サウンドの読み込みを行うクラス
class SoundManager:public Singleton<SoundManager>
{
private:
	//シングルトン
	friend class Singleton<SoundManager>;
	//デバイス
	IDirectSound8* m_pDevice;

	//コピー禁止
	SoundManager(const SoundManager&) {}
protected:
	SoundManager();
public:
	~SoundManager();

	//初期化
	//hWnd:ウインドウハンドル
	void InitDevice(HWND hWnd);

	//シーンのロード
	//FileName:ファイル名
	//OptSound:音声
	void LoadSound(const char* FileName, Sound& OptSound);

	//解放
	void Release();
};

