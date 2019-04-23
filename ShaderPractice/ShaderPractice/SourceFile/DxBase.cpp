#include "stdafx.h"
#include "DxBase.h"
#include"PhysicsWorld.h"
#include"Input.h"
#include"SoundManager.h"

SceneManager& s_SceneManager = SceneManager::GetInstance();

//�R���X�g���N�^
DxBase::DxBase(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd	= hWnd;
	m_hInst = hInst;
}

//�f�X�g���N�^
DxBase::~DxBase()
{
	
}

//������
void DxBase::Init()
{
	ShowWindow(m_hWnd, SW_SHOW);
}

//�X�V
void DxBase::Update()
{
	UpdateWindow(m_hWnd);
}

//���
void DxBase::Release()
{

}

//�R���X�g���N�^
Content::Content(HWND hWnd, HINSTANCE hInst) :DxBase(hWnd, hInst)
{

}

//�f�X�g���N�^
Content::~Content()
{
	
}

//������
void Content::Init()
{
	ShowWindow(m_hWnd, SW_SHOW);

	//�}�l�[�W���[�̏�����
	m_pSceneFactory = make_shared<SceneFactory>();	
	s_SceneManager.Init(m_pSceneFactory,m_hWnd,m_hInst);
	/*s_PhysXWorld.InitPhysics();
	s_InputManager.CreateInput(m_hInst, m_hWnd);
	s_SoundManager.InitDevice(m_hWnd);*/
}

//�X�V
void Content::Update()
{
	//UpdateWindow(m_hWnd);			
	s_SceneManager.SceneUpdate();	//�V�[���̍X�V
	s_SceneManager.Render();		//�V�[���̕`��
}

//���
void Content::Release() 
{
	
}