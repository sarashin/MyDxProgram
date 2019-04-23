#pragma once
#include "Sound.h"

//�T�E���h�̓ǂݍ��݂��s���N���X
class SoundManager:public Singleton<SoundManager>
{
private:
	//�V���O���g��
	friend class Singleton<SoundManager>;
	//�f�o�C�X
	IDirectSound8* m_pDevice;

	//�R�s�[�֎~
	SoundManager(const SoundManager&) {}
protected:
	SoundManager();
public:
	~SoundManager();

	//������
	//hWnd:�E�C���h�E�n���h��
	void InitDevice(HWND hWnd);

	//�V�[���̃��[�h
	//FileName:�t�@�C����
	//OptSound:����
	void LoadSound(const char* FileName, Sound& OptSound);

	//���
	void Release();
};

