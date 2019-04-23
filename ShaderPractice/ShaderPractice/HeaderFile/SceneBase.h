#pragma once
#include"D3Device.h"
#include"SceneBase.h"
#include"StageData.h"

//シーンからのメッセージ
enum SCENE_MESSAGE {
	SCENE_LOOP,
	SCENE_CHANGE,
	SCENE_RELOAD,
};

//シーン数
enum SCENE_NUM
{
	SCENE_TITLE,
	SCENE_GAME_FIRST,
	SCENE_GAME_SECOND,
	SCENE_GAME_THARD,
};

//シーンの状態
enum STATE_MSG
{
	LOOP,
	STOP,
	NEXT,
};

//シーンの仮想クラス
class SceneBase
{
protected:
public:
	//コンストラクタ
	SceneBase() {};
	//デストラクタ
	~SceneBase() {};
	//ロード中かどうか
	bool IsLoading;
	//シーンのロード
	//pRenderer :レンダラー
	//Data:	ステージのデータ
	virtual void Load(shared_ptr<D3DRenderer> pRenderer,StageData Data) = 0;
	//シーンの初期化
	virtual void Init() = 0;
	//シーンの更新
	//pNextScene:次シーンid
	virtual SCENE_MESSAGE Update(DWORD* pNextScene)=0;
	//シーンの描画
	virtual void Render()=0;
	//シーンの解放
	virtual void Release() = 0;
};