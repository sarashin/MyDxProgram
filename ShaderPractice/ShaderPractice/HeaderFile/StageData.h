#pragma once
#include"PhysXActor.h"

class StageData
{
public:
	
	//モデルのパス
	vector<char*>			Models;		
	//何番目のモデルか
	vector<int>				ModelNum;	
	//初期位置
	vector<XMMATRIX>		Transforms;	
	//RigidActorの種類
	vector<int>				Actors;		
	//形状
	vector<int>				Shapes;		
	//形状の大きさ
	vector<XMFLOAT3>		ShapeDatas;	
	//重さ
	vector<float>			Weight;
	//次シーン
	DWORD					NextScene;

	StageData();
	~StageData();

	void First();
	void Second();
	void Thard();
};