#pragma once
#include"ActorObject.h"
#include"RigidStatic.h"
class StaticObject:public ActorObject
{
protected:

	void SetUpFilterGroup(physx::PxRigidActor* Actor, physx::PxU32 FilterGroup, physx::PxU32 FilterMask);
public:
	std::shared_ptr<RigidStatic> m_pActor;

	StaticObject();
	StaticObject(RigidStatic*);
	StaticObject(int ObjectShape, XMVECTOR ShapeSize, Transform Trans);
	~StaticObject();

	void CreateActor(int ObjectShape, XMVECTOR ShapeSize, Transform Trans);
	void Update();
	Transform GetPxTrans();
	void Release()override;
};

