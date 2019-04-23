#pragma once
#pragma comment(lib, "dsound.lib") 
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")

//#include <comdef.h>
//#include <dsound.h>
//#include <tchar.h>

//�T�E���h�֌W�̐e�N���X
class Sound
{
protected:
	//�T�E���h�o�b�t�@
	IDirectSoundBuffer8* m_pSoundBuffer;	
public:
	//�T�E���h�̃Z�b�g
	virtual void SetSound(IDirectSoundBuffer8* pSound);	
	//�Đ�
	virtual void Play() = 0;							
	//�X�g�b�v
	virtual void Stop() = 0;		
	//�R���X�g���N�^
	Sound();
	//�f�X�g���N�^
	~Sound();
};

