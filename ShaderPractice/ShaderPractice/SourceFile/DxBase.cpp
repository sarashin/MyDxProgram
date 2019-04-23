#include "stdafx.h"
#include "DxBase.h"
#include"PhysicsWorld.h"
#include"Input.h"
#include"SoundManager.h"

SceneManager& s_SceneManager = SceneManager::GetInstance();

//コンストラクタ
DxBase::DxBase(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd	= hWnd;
	m_hInst = hInst;
}

//デストラクタ
DxBase::~DxBase()
{
	
}

//初期化
void DxBase::Init()
{
	ShowWindow(m_hWnd, SW_SHOW);
}

//更新
void DxBase::Update()
{
	UpdateWindow(m_hWnd);
}

//解放
void DxBase::Release()
{

}

//コンストラクタ
Content::Content(HWND hWnd, HINSTANCE hInst) :DxBase(hWnd, hInst)
{

}

//デストラクタ
Content::~Content()
{
	
}

//初期化
void Content::Init()
{
	ShowWindow(m_hWnd, SW_SHOW);

	//マネージャーの初期化
	m_pSceneFactory = make_shared<SceneFactory>();	
	s_SceneManager.Init(m_pSceneFactory,m_hWnd,m_hInst);
	/*s_PhysXWorld.InitPhysics();
	s_InputManager.CreateInput(m_hInst, m_hWnd);
	s_SoundManager.InitDevice(m_hWnd);*/
}

//更新
void Content::Update()
{
	//UpdateWindow(m_hWnd);			
	s_SceneManager.SceneUpdate();	//シーンの更新
	s_SceneManager.Render();		//シーンの描画
}

//解放
void Content::Release() 
{
	
}