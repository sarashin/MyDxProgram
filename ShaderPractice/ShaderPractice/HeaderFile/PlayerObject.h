#pragma once
#include "CharactorObject.h"
#include"SoundEffect.h"

//プレイヤーのクラス
class PlayerObject :public CharactorObject
{
protected:

public:
	//ジョイント
	shared_ptr<Joint> m_pJoint;

	//コライダ
	shared_ptr<Collider> m_pCollider;

	//映しているカメラ
	Camera* Flont;
	
	//初期化
	void Init();
	//初期化

	//Trans:初期座標
	void Init(Transform Trans);

	//コンストラクタ
	PlayerObject();

	//コンストラクタ
	//FileName:ファイル名
	//pDevice:デバイスのポインタ
	//Shape:形状
	//Size:形状の大きさ
	//ColliderShape:コライダの形状の大きさ
	//Weight:RigidBodyの重さ
	PlayerObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int Shape, XMVECTOR Size,XMVECTOR ColliderShape,float Weight);

	//コンストラクタ
	//FileName:ファイル名
	//pDevice:デバイスのポインタ
	//Trans:初期座標
	//Shape:形状
	//Size:形状の大きさ
	//ColliderShape:コライダの形状の大きさ
	//Weight:RigidBodyの重さ
	PlayerObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize, XMVECTOR ColliderShape,float Weight);

	//コンストラクタ
	//pMesh:メッシュのポインタ
	//pActor:アクタのポインタ
	PlayerObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidBody> pActor);

	//pMesh:メッシュのポインタ
	//pActor:アクタのポインタ
	//Trans:初期座標
	PlayerObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidBody> pActor, Transform Trans);

	//デストラクタ
	~PlayerObject();

	//更新
	void Update();

	//オブジェクトのコピー
	void CopyObject(PlayerObject* ppObject);

	//コライダのコールバック関数
	//Ptr:thisポインタ
	//pActor:接触したアクタのポインタ
	static void CallBackFanc(void* Ptr,PhysXActor* pActor);

	//ジョイント作成
	void SetJoint(PhysXActor* Actor);

	//解放
	void Release();
};

