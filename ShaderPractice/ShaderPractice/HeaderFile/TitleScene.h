#pragma once
#include"SceneBase.h"

//タイトルシーン
class TitleScene
{
public:
	//コンストラクタ
	TitleScene();
	//デストラクタ
	~TitleScene();

	//ロード
	virtual void Load(shared_ptr<D3DRenderer> pRenderer);
	//初期化
	virtual void Init();
	//更新
	virtual SCENE_MESSAGE Update(DWORD* pNextScene);
	//描画
	virtual void Render();
	//解放
	virtual void Release();
};

