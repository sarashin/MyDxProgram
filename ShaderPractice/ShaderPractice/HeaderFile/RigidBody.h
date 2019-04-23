#pragma once
#include"PhysXActor.h"

//リジッドボディのクラス
class RigidBody:public PhysXActor
{
protected:
	//アクタのフィルタ
	void SetUpFilterGroup(PxRigidActor* Actor, PxU32 FilterGroup, PxU32 FilterMask);
	//アクタのポインタ
	PxRigidDynamic* m_pRigidBody;
public:

	//コンストラクタ
	RigidBody();
	//コンストラクタ
	//pRigidBody:リジットボディのポインタ
	RigidBody(PxRigidDynamic* pRigidBody);
	//デストラクタ
	~RigidBody();

	//アクタに力を加える
	//Speed:速さ
	//Vector:力の向き
	void AddForce(float Speed, XMFLOAT3 Vector);
	void AddForce(float Speed, XMVECTOR Vector);

	//アクタに回転の力を加える
	//Speed:力の速さ
	//Vector:力のベクトル
	void AddTorque(float Speed, XMFLOAT3 Vector);
	void AddTorque(float Speed, XMVECTOR Vector);

	//アクタの取得
	PxRigidActor* GetActor();
	//アクタの設定
	void SetRigidBody(PxRigidDynamic* pRigid);

	//初期化
	void Init();
	//更新
	void Update()override;
	//座標設定
	void SetPxMat(Transform Trans);
	//PhysX上の座標取得
	Transform GetPxTrans() override;
	//力を静止させる
	void ClearAllForce();
	//形状の取得
	//ShapeSize:形状の大きさ
	int GetShape(XMVECTOR& ShapeSize);
	//重さの取得
	float GetWeight();
	
	//解放
	void Release();
};