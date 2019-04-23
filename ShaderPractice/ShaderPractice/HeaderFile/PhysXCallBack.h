#pragma once
#include"Colider.h"
#include"RigidBody.h"

//PhysX�ŌĂ΂��J�X�^���R�[���o�b�N
class PhysXCallBack : public physx::PxSimulationEventCallback
{
public:
	//�o�^�R���C�_�̃|�C���^
	std::vector<Collider*> m_pColliders;
	//�o�^���W�b�g�{�f�B�̃|�C���^
	std::vector<RigidBody*> m_pRigidBodys;

	//�R���X�g���N�^
	PhysXCallBack();
	//�f�X�g���N�^
	~PhysXCallBack();

	//�g���K���m�̐ڐG���Ă΂��R�[���o�b�N
	void onTrigger(physx::PxTriggerPair* Pair, physx::PxU32 Count)override;
	//�N�����Ă΂��R�[���o�b�N
	void onWake(physx::PxActor** Pair, physx::PxU32 Count);
	//��~���Ă΂��R�[���o�b�N
	void onSleep(physx::PxActor **actors, physx::PxU32 count);
	//�A�N�^�̐ڐG���Ă΂��R�[���o�b�N
	void onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs);
	//�s��
	void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count);
	//�s��
	void onAdvance(const physx::PxRigidBody *const *bodyBuffer, const physx::PxTransform *poseBuffer, const physx::PxU32 count);
	//�I�u�W�F�N�g�̃N���A
	void Clear();
	//���
	void Release();
};

