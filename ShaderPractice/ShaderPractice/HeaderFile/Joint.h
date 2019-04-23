#pragma once
#include"Colider.h"
#include"RigidBody.h"

class Joint
{
protected:
	//PhysXジョイントのポインタ
	vector<PxFixedJoint*> m_pJoint;

public:
	//コンストラクタ
	Joint();
	//コンストラクタ
	//pJointActor:ジョイントするアクタのポインタ
	//pJointedActor:ジョイントされるアクタ
	Joint(shared_ptr<PhysXActor> pJointActor, shared_ptr<PhysXActor> pJointedActor);
	~Joint();
	//ジョイントの設定
	void SetJoint(physx::PxFixedJoint* pJoint);
	//解放
	void Release();
};

