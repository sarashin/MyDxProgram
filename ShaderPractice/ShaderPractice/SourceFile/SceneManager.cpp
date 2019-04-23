#include "stdafx.h"
#include "SceneManager.h"
#include "MeshObjectFactory.h"
#include "GameScene.h"
#include"PhysicsWorld.h"
#include"Input.h"
#include"SoundManager.h"
#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))

PhysicsWorld& s_PhysXWorld = PhysicsWorld::GetInstance();
InputManager& s_InputManager = InputManager::GetInstance();
SoundManager& s_SoundManager = SoundManager::GetInstance();

SceneManager::SceneManager()
{
	
}


SceneManager::~SceneManager()
{

}

//�t�F�[�h�C���p�X�v���C�g�쐬
void SceneManager::CreateFadeSprite()
{
	m_pFadeSprite = make_unique<SpriteObject>();
	m_pFadeSprite->Create("BlackBoard.png",m_pRenderer->GetDevice());
	m_pFadeSprite->IsAppear = true;
	m_pFadeSprite->GetMeshObject()->m_Material.Alpha = 0.0f;
	
	m_Camera.SetProjMat(D3DXToRadian(45.0f), 640 / 320, 0.1f, 1000.0f);
	m_Camera.SetViewMat(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_Camera.SetTranslateW(XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)));
}

//������
void SceneManager::Init(shared_ptr<SceneFactory> pFactory,HWND hWnd, HINSTANCE hInst)
{
	m_pFactory = pFactory;
	m_pScene = make_unique<GameScene>();
	SceneMsg = SCENE_TITLE;

	m_pRenderer = make_shared<D3DRenderer>();
	m_pRenderer->CreateDevice(hWnd);

	MeshFactory::GetInstance().Init(m_pRenderer);

	m_pFactory->GetFirstScene(m_pScene);

	CreateFadeSprite();

	s_PhysXWorld.InitPhysics();
	s_InputManager.CreateInput(hInst, hWnd);
	s_SoundManager.InitDevice(hWnd);
	
	IsLoading = true;

	Fade = thread::thread([&]() {this->FadeIn();
	this->Load(this->SceneMsg);
	this->IsLoading = false;
	this->FadeOut();
	});

	Fade.detach();
}

//�V�[���̍X�V
void SceneManager::SceneUpdate()
{
	InputManager::GetInstance().UpdateKeyState();

	//���[�h���łȂ���΍X�V����
	if (IsLoading == false)
	{
		DWORD SceneState = m_pScene->Update(&SceneMsg);

		if (SceneState == SCENE_RELOAD)
		{
			//�V�[���̍ēǂݍ���
			IsLoading = true;
			IsFading = true;
			s_InputManager.InputEnd();

			Fade = thread::thread([&]() {this->FadeIn();
			m_pScene->Init();
			this->IsLoading = false;
			this->FadeOut();
			});

			Fade.detach();
		}
		else if (SceneState == SCENE_CHANGE)
		{
			//�V�[���̕ύX
			IsLoading = true;
			IsFading = true;
			s_InputManager.InputEnd();

			Fade = thread::thread([&]() {this->FadeIn();
										this->Load(this->SceneMsg);
										this->IsLoading = false;
										this->FadeOut();
			});

			Fade.detach();
		}
	}
}

//�t�F�[�h�C��
void SceneManager::FadeIn()
{
	while (m_pFadeSprite->GetMeshObject()->m_Material.Alpha < 1.0)
	{
		m_pFadeSprite->GetMeshObject()->m_Material.Alpha += 0.01f;
		this_thread::sleep_for(chrono::milliseconds(10));
	}

	m_pFadeSprite->GetMeshObject()->m_Material.Alpha = 1.0;	
}

//���[�h
void SceneManager::Load(DWORD SceneName)
{
	m_pFactory->GetNextScene(m_pScene, SceneName);
	m_pScene->Load(m_pRenderer,Stage);
	m_pScene->Init();
}

//�t�F�[�h�A�E�g
void SceneManager::FadeOut()
{
	while (m_pFadeSprite->GetMeshObject()->m_Material.Alpha > 0.0)
	{
		m_pFadeSprite->GetMeshObject()->m_Material.Alpha -= 0.01f;
		this_thread::sleep_for(chrono::milliseconds(10));
	}
	
	m_pFadeSprite->GetMeshObject()->m_Material.Alpha = 0.0;
	InputManager::GetInstance().InputStart();
	IsLoading = false;
}

//�V�[���̕`��
void SceneManager::Render()
{
	//�����_�����O�J�n
	m_pRenderer->RenderStart();

	if (IsLoading == false)
		m_pScene->Render();

	if (m_pFadeSprite->IsEnable == true)
	{
		m_pRenderer->InvalidDepthStencil();//Z�o�b�t�@�𖳌�
		m_pFadeSprite->Draw(m_Camera, m_pRenderer->GetDevContext(),m_pRenderer->m_pDefVShader.Get(), m_pRenderer->m_pDefPShader.Get());
	}

	//�����_�����O�I��
	m_pRenderer->RenderEnd();
}

//���
void SceneManager::Release()
{
	m_pFactory.reset();
	m_pFactory = NULL;

	m_pScene.release();
	m_pScene = NULL;
}