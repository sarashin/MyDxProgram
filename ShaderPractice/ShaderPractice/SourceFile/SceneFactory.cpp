#include "stdafx.h"
#include "SceneFactory.h"
#include "GameScene.h"

SceneFactory::SceneFactory()
{
}


SceneFactory::~SceneFactory()
{
}

//始めのシーン取得
bool SceneFactory::GetFirstScene(unique_ptr<SceneBase>& pScene)
{
	if (pScene != NULL)
	{
		pScene = make_unique<GameScene>();
		return true;
	}

	return false;
}

//次のシーン取得
bool SceneFactory::GetNextScene(unique_ptr<SceneBase>& pScene,DWORD pSceneId)
{
	pScene->Release();
	
	switch (pSceneId)
	{
	case SCENE_TITLE:
		pScene = make_unique<GameScene>();
		return true;
	default:
		return false;
	}
}

//解放
void SceneFactory::Release()
{

}