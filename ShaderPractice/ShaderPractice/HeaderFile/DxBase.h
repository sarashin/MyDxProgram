#pragma once
#include"SceneManager.h"

//�E�C���h�E�ƃR���e���c���֘A�t����N���X
class DxBase
{
protected:
	//�E�C���h�E�n���h��
	HWND		m_hWnd;
	//�C���X�^���X�n���h��
	HINSTANCE	m_hInst;

public:
	//�R���X�g���N�^
	DxBase(HWND hWnd,HINSTANCE hInst);
	//�f�X�g���N�^
	~DxBase();

	//������
	virtual void Init();
	//�X�V
	virtual void Update();
	//���
	virtual void Release();
};

//�R���e���c�̃N���X
class Content : public DxBase
{
protected:
	//�V�[���̐������s��Factory
	shared_ptr<SceneFactory>	m_pSceneFactory;
public:
	//�R���X�g���N�^
	//hWnd : �E�C���h�E�n���h��
	//hInst:�C���X�^���X�n���h��
	Content(HWND hWnd, HINSTANCE hInst);
	//�f�X�g���N�^
	~Content();

	//������
	void Init();
	//�X�V
	void Update();
	//���
	void Release();
};