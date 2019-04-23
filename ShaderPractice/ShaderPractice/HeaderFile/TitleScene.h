#pragma once
#include"SceneBase.h"

//�^�C�g���V�[��
class TitleScene
{
public:
	//�R���X�g���N�^
	TitleScene();
	//�f�X�g���N�^
	~TitleScene();

	//���[�h
	virtual void Load(shared_ptr<D3DRenderer> pRenderer);
	//������
	virtual void Init();
	//�X�V
	virtual SCENE_MESSAGE Update(DWORD* pNextScene);
	//�`��
	virtual void Render();
	//���
	virtual void Release();
};

