#pragma once
#include"Colider.h"
#include"RigidBody.h"

class Joint
{
protected:
	//PhysX�W���C���g�̃|�C���^
	vector<PxFixedJoint*> m_pJoint;

public:
	//�R���X�g���N�^
	Joint();
	//�R���X�g���N�^
	//pJointActor:�W���C���g����A�N�^�̃|�C���^
	//pJointedActor:�W���C���g�����A�N�^
	Joint(shared_ptr<PhysXActor> pJointActor, shared_ptr<PhysXActor> pJointedActor);
	~Joint();
	//�W���C���g�̐ݒ�
	void SetJoint(physx::PxFixedJoint* pJoint);
	//���
	void Release();
};

