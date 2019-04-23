#include "stdafx.h"
#include "SafeArea.h"
#include"PhysicsWorld.h"

SafeArea::SafeArea()
{
}

SafeArea::SafeArea(int Shape, XMVECTOR Size, Transform Trans, shared_ptr<PlayerObject> pPlayer)
{
	m_pActor = make_shared<Collider>();
	PhysicsWorld::GetInstance().CreateColliderBox(m_pActor, Size, Trans);
	m_pActor->SetEnterCallBack(&SafeArea::CallBackFunc, this);
	m_pPlayer = pPlayer;
}

SafeArea::~SafeArea()
{
}

void SafeArea::Init()
{
	IsEnter = false;
}

void SafeArea::CallBackFunc(void* Ptr, PhysXActor* pActor)
{
	reinterpret_cast<SafeArea*>(Ptr)->DeleteObject(pActor);
}

void SafeArea::DeleteObject(PhysXActor* pActor)
{
	if (pActor == m_pPlayer.lock()->m_pActor.get())
	{
		IsEnter = true;
	}
}

bool SafeArea::Update()
{
	m_pActor->Update();

	return IsEnter;
}

void SafeArea::Release()
{
	m_pActor.reset();
	
}