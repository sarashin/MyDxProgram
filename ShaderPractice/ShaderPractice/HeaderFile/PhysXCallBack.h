#pragma once
#include"Colider.h"
#include"RigidBody.h"

//PhysXで呼ばれるカスタムコールバック
class PhysXCallBack : public physx::PxSimulationEventCallback
{
public:
	//登録コライダのポインタ
	std::vector<Collider*> m_pColliders;
	//登録リジットボディのポインタ
	std::vector<RigidBody*> m_pRigidBodys;

	//コンストラクタ
	PhysXCallBack();
	//デストラクタ
	~PhysXCallBack();

	//トリガ同士の接触時呼ばれるコールバック
	void onTrigger(physx::PxTriggerPair* Pair, physx::PxU32 Count)override;
	//起動時呼ばれるコールバック
	void onWake(physx::PxActor** Pair, physx::PxU32 Count);
	//停止時呼ばれるコールバック
	void onSleep(physx::PxActor **actors, physx::PxU32 count);
	//アクタの接触時呼ばれるコールバック
	void onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs);
	//不明
	void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count);
	//不明
	void onAdvance(const physx::PxRigidBody *const *bodyBuffer, const physx::PxTransform *poseBuffer, const physx::PxU32 count);
	//オブジェクトのクリア
	void Clear();
	//解放
	void Release();
};

