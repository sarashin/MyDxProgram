#pragma once
#include"SceneManager.h"

//ウインドウとコンテンツを関連付けるクラス
class DxBase
{
protected:
	//ウインドウハンドル
	HWND		m_hWnd;
	//インスタンスハンドル
	HINSTANCE	m_hInst;

public:
	//コンストラクタ
	DxBase(HWND hWnd,HINSTANCE hInst);
	//デストラクタ
	~DxBase();

	//初期化
	virtual void Init();
	//更新
	virtual void Update();
	//解放
	virtual void Release();
};

//コンテンツのクラス
class Content : public DxBase
{
protected:
	//シーンの生成を行うFactory
	shared_ptr<SceneFactory>	m_pSceneFactory;
public:
	//コンストラクタ
	//hWnd : ウインドウハンドル
	//hInst:インスタンスハンドル
	Content(HWND hWnd, HINSTANCE hInst);
	//デストラクタ
	~Content();

	//初期化
	void Init();
	//更新
	void Update();
	//解放
	void Release();
};