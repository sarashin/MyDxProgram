#pragma once
#include"SceneBase.h"

//シーンの生成を行うクラス
class SceneFactory
{
public:
	//コンストラクタ
	SceneFactory();
	//デストラクタ
	~SceneFactory();

	//最初のシーン取得
	virtual bool GetFirstScene(unique_ptr<SceneBase>& pScene);					
	//次のシーン取得
	virtual bool GetNextScene(unique_ptr<SceneBase>& pScene, DWORD NextScene);	
	//解放
	virtual void Release();														
};

