#pragma once
#pragma comment(lib, "dsound.lib") 
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")

//#include <comdef.h>
//#include <dsound.h>
//#include <tchar.h>

//サウンド関係の親クラス
class Sound
{
protected:
	//サウンドバッファ
	IDirectSoundBuffer8* m_pSoundBuffer;	
public:
	//サウンドのセット
	virtual void SetSound(IDirectSoundBuffer8* pSound);	
	//再生
	virtual void Play() = 0;							
	//ストップ
	virtual void Stop() = 0;		
	//コンストラクタ
	Sound();
	//デストラクタ
	~Sound();
};

