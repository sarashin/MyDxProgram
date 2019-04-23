#pragma once
//#include <d3d11.h>
//#include <d3dx11.h>
#include"D3Device.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#include"SceneBase.h"
#include"SceneFactory.h"

//シーンの更新とフェードインアウトを行うクラス
class SceneManager:public Singleton<SceneManager>
{
private:
	friend class Singleton<SceneManager>;
	//シーンファクトリ
	shared_ptr<SceneFactory>	m_pFactory;	
	//シーン
	unique_ptr<SceneBase>		m_pScene;	
	//ロード中のUI表示用カメラ
	Camera						m_Camera;	
	//レンダラー
	shared_ptr<D3DRenderer>		m_pRenderer;
	//シーンからのメッセージ
	DWORD						SceneMsg;	
	//ステージ
	StageData					Stage;

	//フェードイン、アウトのスレッド
	std::thread Fade;

	SceneManager();
	SceneManager(const SceneManager&) {};

public:
	//ロード中か
	bool IsLoading;	
	//フェードインしているか
	bool IsFading;	
	//シーンを変えるか
	bool IsChange;	

	//ロード中UI
	unique_ptr<SpriteObject>	m_pFadeSprite;				

	//デストラクタ
	~SceneManager();
	//初期化
	//pFactory:シーンのファクトリ
	//hWnd:ウインドウハンドル
	//hInst:インスタンスハンドル
	void Init(shared_ptr<SceneFactory> pFactory,HWND hWnd,HINSTANCE hInst);	

	//現シーンの更新
	void SceneUpdate();				

	//フェードUIの作成
	void CreateFadeSprite();			

	//現シーンの描画
	void Render();							

	//フェードイン
	void FadeIn();						

	//フェードアウト
	void FadeOut();											
	
	//シーンのロード
	//SceneName:シーン名
	void Load(DWORD SceneName);			

	//解放
	void Release();											
};