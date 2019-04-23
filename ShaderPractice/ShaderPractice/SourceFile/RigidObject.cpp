#include "stdafx.h"
#include "RigidObject.h"
#include "PhysicsWorld.h"
using namespace physx;

//�R���X�g���N�^
RigidObject::RigidObject()
{
	m_pActor.reset(new RigidBody);
}

//�R���X�g���N�^
//pActor : �A�N�^�̃|�C���^
RigidObject::RigidObject(RigidBody* pActor)
{
	m_pActor.reset(pActor);
	m_InitMat = pActor->GetPxTrans();
}

//�R���X�g���N�^
//ObjectShape: �`��
//ShapeSize  : �`��̑傫��
//Trans	     : �������W
//Weight     : ����
RigidObject::RigidObject(int ObjectShape, XMVECTOR ShapeSize, Transform Trans, float Weight = 1.0)
{
	CreateActor(ObjectShape,ShapeSize,Trans,Weight);
	m_InitMat = Trans;
}

//�f�X�g���N�^
RigidObject::~RigidObject()
{
	Release();
}

//�`��쐬
//ObjectShape: �`��
//ShapeSize  : �`��̑傫��
//Trans	     : �������W
//Weight     : ����
void RigidObject::CreateActor(int ObjectShape, XMVECTOR ShapeSize,Transform Trans, float Weight = 1.0)
{
	m_pActor.reset();
	m_pActor = make_shared<RigidBody>();
	PhysicsWorld::GetInstance().CreateRigid(m_pActor, ObjectShape, ShapeSize, Trans,Weight);
	m_InitMat = Trans;
}

//������
void RigidObject::Init()
{
	m_pActor->SetPxMat(m_InitMat);
}

//�X�V
void RigidObject::Update()
{

}

//�I�u�W�F�N�g�ɗ͂�������
//speed : �x�N�g���̑傫��
//Vec   : �����x�N�g��
void RigidObject::AddForce(float speed, XMFLOAT3 Vec)
{
	m_pActor->AddForce(speed, Vec);
}

//�I�u�W�F�N�g�ɉ�]����͂�������
//speed : �x�N�g���̑傫��
//Vec   : �����x�N�g��
void RigidObject::AddTorque(float speed, XMFLOAT3 Vec)
{
	m_pActor->AddTorque(speed, Vec);
}

//PhysX�ł̍��W�擾
Transform RigidObject::GetPxTrans()
{
	return m_pActor->GetPxTrans();
}

//���
void RigidObject::Release()
{
	m_pActor.reset();
}