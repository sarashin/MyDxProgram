#include "stdafx.h"
#include "PhysXCallBack.h"
#include"ActorObject.h"
using namespace std;
using namespace physx;

PhysXCallBack::PhysXCallBack()
{
	
}


PhysXCallBack::~PhysXCallBack()
{
}

void PhysXCallBack::onTrigger(physx::PxTriggerPair* Pairs, physx::PxU32 Count)
{
	vector<Collider*>::iterator It = m_pColliders.begin();

	//アクタが同じもの同士なら判定しない
	if (Pairs->triggerActor == Pairs->otherActor && Pairs->triggerActor->userData == Pairs->otherActor->userData)
		return;
	
	while (It != m_pColliders.end())
	{
		if ((*It)->GetActor() != Pairs->triggerActor)
		{
			It++;
			continue;
		}

		if (Pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//触った
			//リストに追加
			(*It)->Contact.push_back((PhysXActor*)(Pairs->otherActor->userData));
			(*It)->Enter.push_back((PhysXActor*)(Pairs->otherActor->userData));
		}
		else if (Pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//離れた
			if ((*It)->Contact.empty())
			{
				It++;
				continue;
			}
			//アクタの探索
			vector<PhysXActor*>::iterator ColIt= (*It)->Contact.begin();

			while (ColIt != (*It)->Contact.end() && (*ColIt) != Pairs->otherActor->userData )
			{
				ColIt++;
			}

			//一致するアクタを削除
			if (ColIt != (*It)->Contact.end() && (*ColIt) == Pairs->otherActor->userData)
			{
				//OnTriggerExit
				(*It)->Contact.erase(ColIt);
				(*It)->Contacted.push_back((PhysXActor*)(Pairs->otherActor->userData));
			}
		}
		
		It++;
	}
}

void PhysXCallBack::onWake(physx::PxActor** Pair, physx::PxU32 Count)
{}

void PhysXCallBack::onSleep(physx::PxActor **actors, physx::PxU32 count)
{}

void PhysXCallBack::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs)
{}

void PhysXCallBack::onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count)
{}

void PhysXCallBack::onAdvance(const physx::PxRigidBody *const *bodyBuffer, const physx::PxTransform *poseBuffer, const physx::PxU32 count)
{}

void PhysXCallBack::Clear()
{
	m_pColliders.clear();
	m_pRigidBodys.clear();
}