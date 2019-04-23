#include"stdafx.h"
#include "SoundManager.h"



SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}

void SoundManager::InitDevice(HWND hWnd)
{
	DirectSoundCreate8(NULL, &m_pDevice, NULL);

	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return;
	}
}

void SoundManager::LoadSound(const char* FName, Sound& OptSound)
{
	WAVEFORMATEX		WaveFormat;					// Wave�t�H�[�}�b�g
	byte*				WaveData = NULL;			// ���̐��f�[�^
	int					DataSize;					// �f�[�^�T�C�Y
	IDirectSoundBuffer8* Sound;

	FILE* fp;
	if (fopen_s(&fp,FName, "rb") != 0)
	{
		return;
	}

	char ChunkId[5] = {};
	char Tmp[5] = {};
	unsigned int ChunkSize = 0;
	fread(ChunkId, sizeof(char) * 4, 1, fp);
	fread(&ChunkSize, sizeof(unsigned int), 1, fp);
	fread(Tmp, sizeof(char) * 4, 1, fp);
	if (strcmp(ChunkId, "RIFF") || strcmp(Tmp, "WAVE")) {
		return;	// WAV�t�@�C������Ȃ�
	}

	// �q�`�����N�ǂݍ���
	bool FmtChunk = false;
	bool DataChunk = false;

	while (1)
	{
		fread(ChunkId, sizeof(char) * 4, 1, fp);
		fread(&ChunkSize, sizeof(unsigned int), 1, fp);
		if (!strcmp(ChunkId, "fmt "))
		{
			if (ChunkSize >= sizeof(WAVEFORMATEX))
			{
				fread(&WaveFormat, sizeof(WAVEFORMATEX), 1, fp);
				int Diff = ChunkSize - sizeof(WAVEFORMATEX);
				fseek(fp, Diff, SEEK_CUR);
			}
			else
			{
				memset(&WaveFormat, 0, sizeof(WAVEFORMATEX));
				fread(&WaveFormat, ChunkSize, 1, fp);
			}
			FmtChunk = true;			
		}
		else if (!strcmp(ChunkId, "data"))
		{
			DataSize = ChunkSize;
			WaveData = new byte[ChunkSize];

			if (fread(WaveData, sizeof(byte), ChunkSize, fp) != ChunkSize)
			{
				fclose(fp);
				return;	// �t�@�C�������Ă���
			}
			DataChunk = true;
		}
		else
		{
			fseek(fp, ChunkSize, SEEK_CUR);

		}

		if (FmtChunk&&DataChunk)
			break;
	}
	fclose(fp);

	DSBUFFERDESC			Desc = {};
	// �`�����l�����ł̕���A���m������1�A�X�e���I��2
	if (WaveFormat.nChannels == 1)
	{
		Desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC;
		Desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else
	{
		Desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
		// | DSBCAPS_CTRLFX;	�G�t�F�N�g��ǉ������ Duplicate �ł��Ȃ�
		Desc.guid3DAlgorithm = GUID_NULL;
	}
	Desc.dwSize = sizeof(DSBUFFERDESC);
	Desc.dwBufferBytes = DataSize;			// ���f�[�^�T�C�Y
	Desc.lpwfxFormat = &WaveFormat;			// �t�H�[�}�b�g

	IDirectSoundBuffer*	pPrimaryBuffer = NULL;	// �v���C�}���o�b�t�@
												// �v���C�}���o�b�t�@�쐬
	m_pDevice->CreateSoundBuffer(&Desc, &pPrimaryBuffer, NULL);
	// �v���C�}���o�b�t�@����Z�J���_���o�b�t�@�쐬
	pPrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&Sound);
	pPrimaryBuffer->Release();	// �Z�J���_���o�b�t�@���쐬�ł���΁A�v���C�}���o�b�t�@�͔j��

	unsigned char* block1 = NULL;
	unsigned char* block2 = NULL;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;
	// �Z�J���_���o�b�t�@�����b�N���ăf�[�^��������
	Sound->Lock(0, DataSize,
		(void**)&block1, &blockSize1, (void**)&block2, &blockSize2, DSBLOCK_ENTIREBUFFER);
	// �Z�J���_���o�b�t�@�ɉ��f�[�^�R�s�[
	memcpy(block1, WaveData, DataSize);
	// �Z�J���_���o�b�t�@���b�N����
	Sound->Unlock(block1, blockSize1, block2, 0);
	OptSound.SetSound(Sound);
}

void SoundManager::Release()
{
	m_pDevice->Release();
	m_pDevice = NULL;
}