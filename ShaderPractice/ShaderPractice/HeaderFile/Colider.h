#pragma once
#include"PhysXActor.h"

typedef void (*FUNC_POINTER)(void* Ptr, PhysXActor* pActor);

class Collider : public PhysXActor
{
protected:
	//PhysX�R���C�_�[�̃|�C���^
	PxRigidStatic* m_pCollider;	
	//�R���C�_�[���������Ă���N���X�̃|�C���^
	void* pObject;
	//�R���C�_�[�ɐG�������Ă΂��֐�
	FUNC_POINTER m_pEnterFunc;
	//�R���C�_�[���痣�ꂽ���ɌĂ΂��֐�
	FUNC_POINTER m_pExitFunc;
	//���I�u�W�F�N�g�Ƃ̃t�B���^
	void SetUpFilterGroup(PxRigidActor* Actor, PxU32 FilterGroup, PxU32 FilterMask);
public:
	//�ڐG���Ă�I�u�W�F�N�g
	vector<PhysXActor*> Contact;
	//�ڐG���Ă����I�u�W�F�N�g
	vector<PhysXActor*> Contacted;
	//�R���C�_�[�ɐڐG�����I�u�W�F�N�g
	vector<PhysXActor*> Enter;

	//�R���X�g���N�^
	Collider();
	//�R���X�g���N�^
	//pActor:�A�N�^�̃|�C���^
	Collider(PxRigidStatic* pActor);
	//�f�X�g���N�^
	~Collider();

	//������
	void Init();
	//�X�V
	void Update();
	//�A�N�^�̐ݒ�
	void SetActor(PxRigidStatic* Collider);
	//�A�N�^�̃|�C���^�̎擾
	PxRigidActor* GetActor();
	//PhysX��̃|�C���^�̎擾
	Transform GetPxTrans();
	//�ʒu�̐ݒ�
	void SetPxTrans(Transform Trans);
	//�R�[���o�b�N�֐��̐ݒ�
	//ExitCall,EnterCall: �R�[���o�b�N�֐�
	//Class				: �R�[���o�b�N�֐�����������N���X
	void SetEnterCallBack(FUNC_POINTER EnterCall,void* Class);
	void SetExitCallBack(FUNC_POINTER EnterCall, void* Class);
	//PhysX�ŌĂ΂��R�[���o�b�N�֐�
	//pTarget:���������I�u�W�F�N�g�̃|�C���^
	void OnCollisionEnter(PhysXActor* pTarget);
	void OnCollisionExit(PhysXActor* pTarget);
	//���
	void Release() override;
};