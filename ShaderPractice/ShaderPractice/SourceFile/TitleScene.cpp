#include "stdafx.h"
#include "TitleScene.h"


TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}


void TitleScene::Init()
{}

void TitleScene::Load(shared_ptr<D3DRenderer> pRenderer)
{}

SCENE_MESSAGE TitleScene::Update(DWORD* pNextScene)
{
	return SCENE_LOOP;
}

void TitleScene::Render()
{}

void TitleScene::Release()
{}