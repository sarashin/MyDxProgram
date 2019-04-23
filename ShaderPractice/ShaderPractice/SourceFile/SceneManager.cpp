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

//フェードイン用スプライト作成
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

//初期化
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

//シーンの更新
void SceneManager::SceneUpdate()
{
	InputManager::GetInstance().UpdateKeyState();

	//ロード中でなければ更新処理
	if (IsLoading == false)
	{
		DWORD SceneState = m_pScene->Update(&SceneMsg);

		if (SceneState == SCENE_RELOAD)
		{
			//シーンの再読み込み
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
			//シーンの変更
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

//フェードイン
void SceneManager::FadeIn()
{
	while (m_pFadeSprite->GetMeshObject()->m_Material.Alpha < 1.0)
	{
		m_pFadeSprite->GetMeshObject()->m_Material.Alpha += 0.01f;
		this_thread::sleep_for(chrono::milliseconds(10));
	}

	m_pFadeSprite->GetMeshObject()->m_Material.Alpha = 1.0;	
}

//ロード
void SceneManager::Load(DWORD SceneName)
{
	m_pFactory->GetNextScene(m_pScene, SceneName);
	m_pScene->Load(m_pRenderer,Stage);
	m_pScene->Init();
}

//フェードアウト
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

//シーンの描画
void SceneManager::Render()
{
	//レンダリング開始
	m_pRenderer->RenderStart();

	if (IsLoading == false)
		m_pScene->Render();

	if (m_pFadeSprite->IsEnable == true)
	{
		m_pRenderer->InvalidDepthStencil();//Zバッファを無効
		m_pFadeSprite->Draw(m_Camera, m_pRenderer->GetDevContext(),m_pRenderer->m_pDefVShader.Get(), m_pRenderer->m_pDefPShader.Get());
	}

	//レンダリング終了
	m_pRenderer->RenderEnd();
}

//解放
void SceneManager::Release()
{
	m_pFactory.reset();
	m_pFactory = NULL;

	m_pScene.release();
	m_pScene = NULL;
}