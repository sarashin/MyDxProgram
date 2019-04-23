#pragma once
#include"PhysXActor.h"

//���W�b�h�{�f�B�̃N���X
class RigidBody:public PhysXActor
{
protected:
	//�A�N�^�̃t�B���^
	void SetUpFilterGroup(PxRigidActor* Actor, PxU32 FilterGroup, PxU32 FilterMask);
	//�A�N�^�̃|�C���^
	PxRigidDynamic* m_pRigidBody;
public:

	//�R���X�g���N�^
	RigidBody();
	//�R���X�g���N�^
	//pRigidBody:���W�b�g�{�f�B�̃|�C���^
	RigidBody(PxRigidDynamic* pRigidBody);
	//�f�X�g���N�^
	~RigidBody();

	//�A�N�^�ɗ͂�������
	//Speed:����
	//Vector:�͂̌���
	void AddForce(float Speed, XMFLOAT3 Vector);
	void AddForce(float Speed, XMVECTOR Vector);

	//�A�N�^�ɉ�]�̗͂�������
	//Speed:�͂̑���
	//Vector:�͂̃x�N�g��
	void AddTorque(float Speed, XMFLOAT3 Vector);
	void AddTorque(float Speed, XMVECTOR Vector);

	//�A�N�^�̎擾
	PxRigidActor* GetActor();
	//�A�N�^�̐ݒ�
	void SetRigidBody(PxRigidDynamic* pRigid);

	//������
	void Init();
	//�X�V
	void Update()override;
	//���W�ݒ�
	void SetPxMat(Transform Trans);
	//PhysX��̍��W�擾
	Transform GetPxTrans() override;
	//�͂�Î~������
	void ClearAllForce();
	//�`��̎擾
	//ShapeSize:�`��̑傫��
	int GetShape(XMVECTOR& ShapeSize);
	//�d���̎擾
	float GetWeight();
	
	//���
	void Release();
};