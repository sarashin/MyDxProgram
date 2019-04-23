#pragma once
#include "SceneBase.h"
#include"CharactorObject.h"
#include"StageObject.h"
#include"PlayerObject.h"
#include"SafeArea.h"

//ゲームのシーン
class GameScene :public SceneBase
{
	//操作できるオブジェクト
	shared_ptr<PlayerObject>		m_Player;
	//物理オブジェクト
	vector<unique_ptr<CharactorObject>>	m_pCharactor;
	//スタティックオブジェクト
	vector<unique_ptr<StageObject>>	m_StageObject;
	//エリア
	vector<unique_ptr<SafeArea>>	m_pSafeArea;
	//レンダラー
	shared_ptr<D3DRenderer>			m_pRenderer;
	//カメラ
	Camera							m_Camera;
	//ライト
	vector<Light>					m_Light;

public:
	//コンストラクタ
	GameScene();
	//デストラクタ
	~GameScene();

	//シーンのロード
	//Data
	//pRenderer
	void Load(shared_ptr<D3DRenderer> pRenderer,StageData Data);
	//シーンの初期化
	void Init();
	//シーンの更新
	SCENE_MESSAGE Update(DWORD* pNextScene);
	//レンダリング
	void Render();
	//解放
	void Release();
};

