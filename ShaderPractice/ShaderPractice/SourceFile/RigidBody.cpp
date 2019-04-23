#include"stdafx.h"
#include "RigidBody.h"
#include"PhysicsWorld.h"
using namespace physx;


RigidBody::RigidBody()
{
	m_pRigidBody = NULL;
}

RigidBody::RigidBody(PxRigidDynamic* pRigid)
{
	m_pRigidBody = pRigid;
}

RigidBody::~RigidBody()
{
	
}

void RigidBody::Init()
{
	if (m_pRigidBody == NULL)
		return;
	m_pRigidBody->setMaxAngularVelocity(0.65f);
	m_pRigidBody->setMaxDepenetrationVelocity(3.0f);
}

void RigidBody::Update()
{
	
}

void RigidBody::AddForce(float Speed, XMFLOAT3 Vec)
{
	PxVec3 Force;

	Force.x = Vec.x*Speed;
	Force.y = Vec.y*Speed;
	Force.z = Vec.z*Speed;

	m_pRigidBody->addForce(Force, PxForceMode::eACCELERATION);
}

void RigidBody::AddForce(float Speed, XMVECTOR Vec)
{
	PxVec3 Force;
	XMFLOAT3 Add;
	XMStoreFloat3(&Add, Vec);

	AddForce(Speed, Add);
}

void RigidBody::AddTorque(float Speed, XMFLOAT3 Vec)
{
	if (m_pRigidBody == NULL)
		return;
	PxVec3 Force;

	Force.x = Vec.x*Speed;
	Force.y = Vec.y*Speed;
	Force.z = Vec.z*Speed;

	m_pRigidBody->addTorque(Force, PxForceMode::eVELOCITY_CHANGE);
}

void RigidBody::AddTorque(float Speed, XMVECTOR Vec)
{
	PxVec3 Force;
	XMFLOAT3 Add;
	XMStoreFloat3(&Add, Vec);

	AddTorque(Speed, Add);
}

PxRigidActor* RigidBody::GetActor()
{
	return m_pRigidBody;
}

void RigidBody::SetRigidBody(PxRigidDynamic* pRigid)
{
	m_pRigidBody = pRigid;
}

void RigidBody::SetPxMat(Transform Trans)
{
	if (m_pRigidBody == NULL)
		return;
	//Transformから要素を取り出す
	//回転
	XMVECTOR Quat=XMQuaternionRotationMatrix(Trans.GetMat());
	XMFLOAT4 Rotation;
	XMStoreFloat4(&Rotation, Quat);
	//位置
	XMFLOAT3 Position;
	Position.x = Trans.TransformMatrix._41;
	Position.y = Trans.TransformMatrix._42;
	Position.z = Trans.TransformMatrix._43;

	//キャスト
	PxQuat PxRotation;
	PxRotation.x = Rotation.x;
	PxRotation.y = Rotation.y;
	PxRotation.z = Rotation.z;
	PxRotation.w = Rotation.w;

	PxVec3 PxPosition;
	PxPosition.x = Position.x;
	PxPosition.y = Position.y;
	PxPosition.z = Position.z;

	PxTransform PxTrans = PxTransform(PxPosition, PxRotation);
	m_pRigidBody->setGlobalPose(PxTrans);
}

Transform RigidBody::GetPxTrans()
{
	if (m_pRigidBody == NULL)
	{
		return XMMatrixIdentity();
	}

	PxTransform PxTrans;
	PxTrans = m_pRigidBody->getGlobalPose();
	//位置　XMFLOAT3にキャスト
	PxVec3 PxPos = PxTrans.p;
	XMFLOAT3 Pos;
	Pos.x = PxPos.x;
	Pos.y = PxPos.y;
	Pos.z = PxPos.z;
	//回転　XMVECTORにキャスト
	PxQuat PxRotate = PxTrans.q;
	XMVECTOR Quatanion;
	XMFLOAT4 Rotate;
	Rotate.x = PxRotate.x;
	Rotate.y = PxRotate.y;
	Rotate.z = PxRotate.z;
	Rotate.w = PxRotate.w;
	Quatanion = XMLoadFloat4(&Rotate);

	XMMATRIX TransMat = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	XMMATRIX RotateMat = XMMatrixRotationQuaternion(Quatanion);

	return Transform(RotateMat*TransMat);
}

void RigidBody::SetUpFilterGroup(physx::PxRigidActor* Actor, physx::PxU32 FilterGroup, physx::PxU32 FilterMask)
{
	PxFilterData FilterData;
	FilterData.word0 = FilterGroup;
	FilterData.word1 = FilterMask;

	const PxU32 ShapeNum = m_pRigidBody->getNbShapes();

	for (int i = 0; i < ShapeNum; i++)
	{
		PxShape* Shape;
		m_pRigidBody->getShapes(&Shape, 1, i);
		Shape->setSimulationFilterData(FilterData);
		Shape->release();
	}
}

void RigidBody::ClearAllForce()
{
	m_pRigidBody->putToSleep();
	m_pRigidBody->wakeUp();
}

int RigidBody::GetShape(XMVECTOR& pShapeSize)
{
	if (m_pRigidBody == NULL)
		return -1;

	PxShape* Shape;
	PxGeometryHolder Holder;
	m_pRigidBody->getShapes(&Shape,1);
	Holder = Shape->getGeometry();
	PxGeometry* Geo;
	PxSphereGeometry Sphere;
	PxBoxGeometry Box;
	float Rad;
	XMFLOAT3 Vec;
	
	switch (Holder.getType())
	{
	case PxGeometryType::eSPHERE :
		Sphere = Holder.sphere();
		Rad = Sphere.radius;
		pShapeSize = XMLoadFloat(&Rad);
		return 1;

	case PxGeometryType::eBOX:
		Box = Holder.box();
		Vec.x = Box.halfExtents.x;
		Vec.y = Box.halfExtents.y;
		Vec.z = Box.halfExtents.z;
		pShapeSize = XMLoadFloat(&Rad);
		return 0;
	default:
		break;
	}
}

float RigidBody::GetWeight()
{
	return m_pRigidBody->getMass();
}

void RigidBody::Release()
{
	if (m_pRigidBody != NULL)
	{
		PhysicsWorld::GetInstance().GetScene()->removeActor(*m_pRigidBody);
		m_pRigidBody->release();
		m_pRigidBody = NULL;
	}
}