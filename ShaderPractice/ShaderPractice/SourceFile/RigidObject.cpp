#include "stdafx.h"
#include "RigidObject.h"
#include "PhysicsWorld.h"
using namespace physx;

//コンストラクタ
RigidObject::RigidObject()
{
	m_pActor.reset(new RigidBody);
}

//コンストラクタ
//pActor : アクタのポインタ
RigidObject::RigidObject(RigidBody* pActor)
{
	m_pActor.reset(pActor);
	m_InitMat = pActor->GetPxTrans();
}

//コンストラクタ
//ObjectShape: 形状
//ShapeSize  : 形状の大きさ
//Trans	     : 初期座標
//Weight     : 質量
RigidObject::RigidObject(int ObjectShape, XMVECTOR ShapeSize, Transform Trans, float Weight = 1.0)
{
	CreateActor(ObjectShape,ShapeSize,Trans,Weight);
	m_InitMat = Trans;
}

//デストラクタ
RigidObject::~RigidObject()
{
	Release();
}

//形状作成
//ObjectShape: 形状
//ShapeSize  : 形状の大きさ
//Trans	     : 初期座標
//Weight     : 質量
void RigidObject::CreateActor(int ObjectShape, XMVECTOR ShapeSize,Transform Trans, float Weight = 1.0)
{
	m_pActor.reset();
	m_pActor = make_shared<RigidBody>();
	PhysicsWorld::GetInstance().CreateRigid(m_pActor, ObjectShape, ShapeSize, Trans,Weight);
	m_InitMat = Trans;
}

//初期化
void RigidObject::Init()
{
	m_pActor->SetPxMat(m_InitMat);
}

//更新
void RigidObject::Update()
{

}

//オブジェクトに力を加える
//speed : ベクトルの大きさ
//Vec   : 方向ベクトル
void RigidObject::AddForce(float speed, XMFLOAT3 Vec)
{
	m_pActor->AddForce(speed, Vec);
}

//オブジェクトに回転する力を加える
//speed : ベクトルの大きさ
//Vec   : 方向ベクトル
void RigidObject::AddTorque(float speed, XMFLOAT3 Vec)
{
	m_pActor->AddTorque(speed, Vec);
}

//PhysXでの座標取得
Transform RigidObject::GetPxTrans()
{
	return m_pActor->GetPxTrans();
}

//解放
void RigidObject::Release()
{
	m_pActor.reset();
}