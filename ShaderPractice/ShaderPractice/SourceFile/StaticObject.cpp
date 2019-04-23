#include "stdafx.h"
#include "StaticObject.h"
#include "PhysicsWorld.h"


StaticObject::StaticObject()
{
	m_pActor = make_shared<RigidStatic>();
}

StaticObject::StaticObject(RigidStatic* pStatic)
{
	m_pActor.reset(pStatic);
}

StaticObject::StaticObject(int ObjectShape, XMVECTOR ShapeSize, Transform Trans)
{
	m_pActor.reset();
	m_pActor = make_shared<RigidStatic>();
	CreateActor(ObjectShape, ShapeSize, Trans);
}

StaticObject::~StaticObject()
{
	Release();
}

void StaticObject::CreateActor(int ObjectShape, XMVECTOR ShapeSize, Transform Trans)
{
	PhysicsWorld::GetInstance().CreateStatic(m_pActor, ObjectShape, ShapeSize, Trans);
}

void StaticObject::Update()
{

}

Transform StaticObject::GetPxTrans()
{
	return m_pActor->GetPxTrans();
}

void StaticObject::Release()
{
	m_pActor.reset();
}