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
	WAVEFORMATEX		WaveFormat;					// Waveフォーマット
	byte*				WaveData = NULL;			// 音の生データ
	int					DataSize;					// データサイズ
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
		return;	// WAVファイルじゃない
	}

	// 子チャンク読み込み
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
				return;	// ファイルが壊れている
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
	// チャンネル数での分岐、モノラルは1、ステレオは2
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
		// | DSBCAPS_CTRLFX;	エフェクトを追加すると Duplicate できない
		Desc.guid3DAlgorithm = GUID_NULL;
	}
	Desc.dwSize = sizeof(DSBUFFERDESC);
	Desc.dwBufferBytes = DataSize;			// 音データサイズ
	Desc.lpwfxFormat = &WaveFormat;			// フォーマット

	IDirectSoundBuffer*	pPrimaryBuffer = NULL;	// プライマリバッファ
												// プライマリバッファ作成
	m_pDevice->CreateSoundBuffer(&Desc, &pPrimaryBuffer, NULL);
	// プライマリバッファからセカンダリバッファ作成
	pPrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&Sound);
	pPrimaryBuffer->Release();	// セカンダリバッファが作成できれば、プライマリバッファは破棄

	unsigned char* block1 = NULL;
	unsigned char* block2 = NULL;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;
	// セカンダリバッファをロックしてデータ書き込み
	Sound->Lock(0, DataSize,
		(void**)&block1, &blockSize1, (void**)&block2, &blockSize2, DSBLOCK_ENTIREBUFFER);
	// セカンダリバッファに音データコピー
	memcpy(block1, WaveData, DataSize);
	// セカンダリバッファロック解除
	Sound->Unlock(block1, blockSize1, block2, 0);
	OptSound.SetSound(Sound);
}

void SoundManager::Release()
{
	m_pDevice->Release();
	m_pDevice = NULL;
}