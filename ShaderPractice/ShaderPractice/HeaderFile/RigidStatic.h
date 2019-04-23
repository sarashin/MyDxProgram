#pragma once
#include"PhysXActor.h"

//PhysX�X�^�e�B�b�N
class RigidStatic:public PhysXActor
{
	//PhysX�X�^�e�B�b�N�̃|�C���^
	physx::PxRigidStatic* m_pRigidStatic;
	//�t�B���^�ݒ�
	void SetUpFilterGroup(PxRigidActor* Actor, PxU32 FilterGroup, PxU32 FilterMask);
public:
	//�R���X�g���N�^
	RigidStatic();
	//�R���X�g���N�^
	//PhysX�X�^�e�B�b�N�̃|�C���^
	RigidStatic(PxRigidStatic* pRigidStatic);
	//�f�X�g���N�^
	~RigidStatic();

	//������
	void Init();
	//�X�V
	void Update() override;
	//�A�N�^�̐ݒ�
	//pStatic:PhysX�X�^�e�B�b�N�̃|�C���^
	void SetActor(PxRigidStatic* pStatic);
	//PhysX��̍��W�擾
	Transform GetPxTrans() override;
	//���W�ݒ�
	void SetPxTrans(Transform Trans);
	//�A�N�^�̎擾
	PxRigidActor* GetActor();
	//���
	void Release()override;
};

