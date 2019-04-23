#pragma once
#include "MeshObject.h"
#include"StaticObject.h"

class StageObject :public MeshObject,public StaticObject
{
	
public:
	//コンストラクタ
	StageObject();

	//コンストラクタ
	//FileName:ファイル名
	//pDevice:デバイスのポインタ
	//Shape:形状
	//Size:形状の大きさ
	StageObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int Shape, XMVECTOR Size);

	//コンストラクタ
	//FileName:ファイル名
	//pDevice:デバイスのポインタ
	//Trans:初期座標
	//Shape:形状
	//Size:形状の大きさ
	StageObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize);

	//コンストラクタ
	//pMesh:メッシュのポインタ
	//pActor:アクタのポインタ
	StageObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidStatic> pActor);

	//コンストラクタ
	//pMesh:メッシュのポインタ
	//pActor:アクタのポインタ
	//Trans:初期座標
	StageObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidStatic> pActor, Transform Trans);

	//デストラクタ
	~StageObject();

	//初期化
	void Init(XMVECTOR Pos);
	//更新
	void Update();
	//コピー
	void CopyObject(StageObject** Object);
	//解放
	void Release()override;
};

