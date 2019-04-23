#pragma once
#include "ActorObject.h"
#include "RigidBody.h"

//���W�b�h�{�f�B�̃N���X
class RigidObject :public ActorObject
{
public:
	//�A�N�^�̃|�C���^
	std::shared_ptr<RigidBody> m_pActor;

	//�R���X�g���N�^
	RigidObject();
	//�R���X�g���N�^
	//pActor:�A�N�^�̃|�C���^
	RigidObject(RigidBody* pActor);
	//�R���X�g���N�^
	//Shape:�`��
	//ShapeSize:�`��̑傫��
	//Trans:�������W
	//Weight:�d��
	RigidObject(int Shape, XMVECTOR Size, Transform Trans,float Weight);
	//�f�X�g���N�^
	~RigidObject();

	//�A�N�^�̍쐬
	void CreateActor(int ObjectShape, XMVECTOR ShapeSize, Transform Trans, float Weight);
	//������
	void Init();
	//�X�V
	void Update();
	//�͂�������
	void AddForce(float speed, XMFLOAT3 Vec);
	//��]�̗͂�������
	void AddTorque(float speed,XMFLOAT3 Vec);
	//PhysX��̍��W�擾
	Transform GetPxTrans()override;
	//���
	void Release()override;
};

