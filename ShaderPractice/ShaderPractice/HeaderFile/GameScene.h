#pragma once
#include "SceneBase.h"
#include"CharactorObject.h"
#include"StageObject.h"
#include"PlayerObject.h"
#include"SafeArea.h"

//�Q�[���̃V�[��
class GameScene :public SceneBase
{
	//����ł���I�u�W�F�N�g
	shared_ptr<PlayerObject>		m_Player;
	//�����I�u�W�F�N�g
	vector<unique_ptr<CharactorObject>>	m_pCharactor;
	//�X�^�e�B�b�N�I�u�W�F�N�g
	vector<unique_ptr<StageObject>>	m_StageObject;
	//�G���A
	vector<unique_ptr<SafeArea>>	m_pSafeArea;
	//�����_���[
	shared_ptr<D3DRenderer>			m_pRenderer;
	//�J����
	Camera							m_Camera;
	//���C�g
	vector<Light>					m_Light;

public:
	//�R���X�g���N�^
	GameScene();
	//�f�X�g���N�^
	~GameScene();

	//�V�[���̃��[�h
	//Data
	//pRenderer
	void Load(shared_ptr<D3DRenderer> pRenderer,StageData Data);
	//�V�[���̏�����
	void Init();
	//�V�[���̍X�V
	SCENE_MESSAGE Update(DWORD* pNextScene);
	//�����_�����O
	void Render();
	//���
	void Release();
};

