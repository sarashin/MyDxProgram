#include "stdafx.h"
#include "ColliderObject.h"
#include"PhysicsWorld.h"

//�R���X�g���N�^
ColliderObject::ColliderObject()
{
}

ColliderObject::ColliderObject(Collider* pColider)
{
	m_pActor.reset(pColider);
}

ColliderObject::ColliderObject(int ObjectShape, XMVECTOR ShapeSize, Transform Trans)
{
	CreateActor(ObjectShape, ShapeSize, Trans);
}

//�f�X�g���N�^
ColliderObject::~ColliderObject()
{
}

//�A�N�^�̍쐬
void ColliderObject::CreateActor(int ObjectShape, XMVECTOR ShapeSize, Transform Trans)
{
	PhysicsWorld::GetInstance().CreateCollider(m_pActor, ObjectShape, ShapeSize, Trans);
}

//�X�V
void ColliderObject::Update()
{
}

//�R���C�_�̎擾
Collider* ColliderObject::GetCollider()
{
	return m_pActor.get();
}

//PhysX��̍��W�擾
Transform ColliderObject::GetPxTrans()
{
	return m_pActor->GetPxTrans();
}

//���
void ColliderObject::Release()
{
	m_pActor.reset();
}