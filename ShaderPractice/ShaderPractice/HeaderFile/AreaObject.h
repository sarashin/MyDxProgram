#pragma once
#include "MeshObject.h"
#include"ColliderObject.h"

class AreaObject : public MeshObject, public ColliderObject
{
public:
	//コンストラクタ
	AreaObject();

	//コンストラクタ
	//FileName  : ファイル名
	//pDevice   : デバイス
	//Trans     : 初期座標
	//Shape     : PhysX上の形状
	//ShapeSize : 形状の大きさ
	AreaObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int ObjectShape, XMVECTOR ShapeSize);

	//コンストラクタ
	//FileName  : ファイル名
	//pDevice   : デバイス
	//Shape     : PhysXの形状
	//ShapeSize : 形状の大きさ
	AreaObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize);

	//コンストラクタ
	//pMesh : メッシュのポインタ
	//pActor: PhysXのアクタのポインタ
	AreaObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<Collider> pActor);

	//コンストラクタ
	//pMesh : メッシュのポインタ
	//pActor: PhysXのアクタのポインタ
	//Trans : 初期座標
	AreaObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<Collider> pActor, Transform Trans);

	//デストラクタ
	~AreaObject();

	//初期化
	//Pos : 座標
	void Init(XMVECTOR Pos);
	//更新
	void Update();
	//コピー
	void CopyObject(AreaObject** Object);
	//解放
	void Release()override;
};

