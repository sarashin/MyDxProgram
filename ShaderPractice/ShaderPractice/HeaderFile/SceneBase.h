#pragma once
#include"D3Device.h"
#include"SceneBase.h"
#include"StageData.h"

//�V�[������̃��b�Z�[�W
enum SCENE_MESSAGE {
	SCENE_LOOP,
	SCENE_CHANGE,
	SCENE_RELOAD,
};

//�V�[����
enum SCENE_NUM
{
	SCENE_TITLE,
	SCENE_GAME_FIRST,
	SCENE_GAME_SECOND,
	SCENE_GAME_THARD,
};

//�V�[���̏��
enum STATE_MSG
{
	LOOP,
	STOP,
	NEXT,
};

//�V�[���̉��z�N���X
class SceneBase
{
protected:
public:
	//�R���X�g���N�^
	SceneBase() {};
	//�f�X�g���N�^
	~SceneBase() {};
	//���[�h�����ǂ���
	bool IsLoading;
	//�V�[���̃��[�h
	//pRenderer :�����_���[
	//Data:	�X�e�[�W�̃f�[�^
	virtual void Load(shared_ptr<D3DRenderer> pRenderer,StageData Data) = 0;
	//�V�[���̏�����
	virtual void Init() = 0;
	//�V�[���̍X�V
	//pNextScene:���V�[��id
	virtual SCENE_MESSAGE Update(DWORD* pNextScene)=0;
	//�V�[���̕`��
	virtual void Render()=0;
	//�V�[���̉��
	virtual void Release() = 0;
};