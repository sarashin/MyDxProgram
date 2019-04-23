#include "stdafx.h"
#include "Joint.h"
#include"PhysicsWorld.h"
using namespace physx;

Joint::Joint()
{
	m_pJoint = vector<PxFixedJoint*>();
}

Joint::Joint(shared_ptr<PhysXActor> pJointActor, shared_ptr<PhysXActor> pJointedActor)
{
	m_pJoint = vector<PxFixedJoint*>();
}

Joint::~Joint()
{
	Release();
}

void Joint::SetJoint(PxFixedJoint* pJoint)
{
	m_pJoint.push_back(pJoint);
}

void Joint::Release()
{
	if (!m_pJoint.empty())
	{
		for(int i=0;i<m_pJoint.size();i++)
			m_pJoint[i]->release();
	}
}