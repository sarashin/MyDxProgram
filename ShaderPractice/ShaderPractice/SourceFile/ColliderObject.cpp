#include "stdafx.h"
#include "ColliderObject.h"
#include"PhysicsWorld.h"

//コンストラクタ
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

//デストラクタ
ColliderObject::~ColliderObject()
{
}

//アクタの作成
void ColliderObject::CreateActor(int ObjectShape, XMVECTOR ShapeSize, Transform Trans)
{
	PhysicsWorld::GetInstance().CreateCollider(m_pActor, ObjectShape, ShapeSize, Trans);
}

//更新
void ColliderObject::Update()
{
}

//コライダの取得
Collider* ColliderObject::GetCollider()
{
	return m_pActor.get();
}

//PhysX上の座標取得
Transform ColliderObject::GetPxTrans()
{
	return m_pActor->GetPxTrans();
}

//解放
void ColliderObject::Release()
{
	m_pActor.reset();
}