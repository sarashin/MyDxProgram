#pragma once
#include"PhysXActor.h"

//PhysXスタティック
class RigidStatic:public PhysXActor
{
	//PhysXスタティックのポインタ
	physx::PxRigidStatic* m_pRigidStatic;
	//フィルタ設定
	void SetUpFilterGroup(PxRigidActor* Actor, PxU32 FilterGroup, PxU32 FilterMask);
public:
	//コンストラクタ
	RigidStatic();
	//コンストラクタ
	//PhysXスタティックのポインタ
	RigidStatic(PxRigidStatic* pRigidStatic);
	//デストラクタ
	~RigidStatic();

	//初期化
	void Init();
	//更新
	void Update() override;
	//アクタの設定
	//pStatic:PhysXスタティックのポインタ
	void SetActor(PxRigidStatic* pStatic);
	//PhysX上の座標取得
	Transform GetPxTrans() override;
	//座標設定
	void SetPxTrans(Transform Trans);
	//アクタの取得
	PxRigidActor* GetActor();
	//解放
	void Release()override;
};

