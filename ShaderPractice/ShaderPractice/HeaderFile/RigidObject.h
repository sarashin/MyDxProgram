#pragma once
#include "ActorObject.h"
#include "RigidBody.h"

//リジッドボディのクラス
class RigidObject :public ActorObject
{
public:
	//アクタのポインタ
	std::shared_ptr<RigidBody> m_pActor;

	//コンストラクタ
	RigidObject();
	//コンストラクタ
	//pActor:アクタのポインタ
	RigidObject(RigidBody* pActor);
	//コンストラクタ
	//Shape:形状
	//ShapeSize:形状の大きさ
	//Trans:初期座標
	//Weight:重さ
	RigidObject(int Shape, XMVECTOR Size, Transform Trans,float Weight);
	//デストラクタ
	~RigidObject();

	//アクタの作成
	void CreateActor(int ObjectShape, XMVECTOR ShapeSize, Transform Trans, float Weight);
	//初期化
	void Init();
	//更新
	void Update();
	//力を加える
	void AddForce(float speed, XMFLOAT3 Vec);
	//回転の力を加える
	void AddTorque(float speed,XMFLOAT3 Vec);
	//PhysX上の座標取得
	Transform GetPxTrans()override;
	//解放
	void Release()override;
};

