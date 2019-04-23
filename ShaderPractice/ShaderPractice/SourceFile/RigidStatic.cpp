#include "stdafx.h"
#include "RigidStatic.h"
#include"PhysicsWorld.h"
using namespace physx;


RigidStatic::RigidStatic()
{
	m_pRigidStatic = NULL;
}

RigidStatic::RigidStatic(PxRigidStatic* pStatic)
{
	m_pRigidStatic = pStatic;
	SetUpFilterGroup(m_pRigidStatic, FilterGroup::eLAND, FilterGroup::eBOX);
}

RigidStatic::~RigidStatic()
{
	Release();
}

void RigidStatic::Init()
{}

void RigidStatic::Update()
{
}

void RigidStatic::SetActor(PxRigidStatic* Static)
{
	m_pRigidStatic = Static;
}

Transform RigidStatic::GetPxTrans()
{
	if (m_pRigidStatic == NULL)
	{
		return XMMatrixIdentity();
	}

	PxTransform PxTrans;
	PxTrans = m_pRigidStatic->getGlobalPose();
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

void RigidStatic::SetPxTrans(Transform Trans)
{
	//Transformから要素を取り出す
	//回転
	XMVECTOR Quat = XMQuaternionRotationMatrix(Trans.GetMat());
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
	m_pRigidStatic->setGlobalPose(PxTrans);
}

void RigidStatic::SetUpFilterGroup(physx::PxRigidActor* Actor, physx::PxU32 FilterGroup, physx::PxU32 FilterMask)
{
	PxFilterData FilterData;
	FilterData.word0 = FilterGroup;
	FilterData.word1 = FilterMask;

	const PxU32 ShapeNum = m_pRigidStatic->getNbShapes();

	for (int i = 0; i < ShapeNum; i++)
	{
		PxShape* Shape;
		m_pRigidStatic->getShapes(&Shape, 1, i);
		Shape->setSimulationFilterData(FilterData);
	}
}

PxRigidActor* RigidStatic::GetActor()
{
	return m_pRigidStatic;
}

void RigidStatic::Release()
{
	if (m_pRigidStatic != NULL)
	{
		PhysicsWorld::GetInstance().GetScene()->removeActor(*m_pRigidStatic);
		m_pRigidStatic->release();
		m_pRigidStatic = NULL;
	}
}