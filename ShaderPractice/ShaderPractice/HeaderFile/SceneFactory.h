#pragma once
#include"SceneBase.h"

//�V�[���̐������s���N���X
class SceneFactory
{
public:
	//�R���X�g���N�^
	SceneFactory();
	//�f�X�g���N�^
	~SceneFactory();

	//�ŏ��̃V�[���擾
	virtual bool GetFirstScene(unique_ptr<SceneBase>& pScene);					
	//���̃V�[���擾
	virtual bool GetNextScene(unique_ptr<SceneBase>& pScene, DWORD NextScene);	
	//���
	virtual void Release();														
};

