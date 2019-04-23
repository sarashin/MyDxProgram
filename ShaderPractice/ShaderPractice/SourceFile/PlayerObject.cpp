#include "stdafx.h"
#include "PlayerObject.h"
#include"PhysicsWorld.h"
#include"Input.h"

InputManager& I = InputManager::GetInstance();


PlayerObject::PlayerObject():CharactorObject()
{
	m_pCollider->SetEnterCallBack(&PlayerObject::CallBackFanc, this);
	m_pJoint = make_shared<Joint>();
	m_pActor->Init();
}

PlayerObject::PlayerObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidBody> pActor) : CharactorObject(pMesh,*pActor)
{
	m_pCollider->SetEnterCallBack(&PlayerObject::CallBackFanc, this);
	m_pJoint = make_shared<Joint>();
	m_pActor->Init();
}

PlayerObject::PlayerObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidBody> pActor, Transform Trans):CharactorObject(pMesh,*pActor,Trans)
{
	m_pCollider->SetEnterCallBack(&PlayerObject::CallBackFanc, this);
	m_pJoint = make_shared<Joint>();
	m_pActor->Init();
}

PlayerObject::PlayerObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int ObjectShape, XMVECTOR ShapeSize, XMVECTOR ColliderShape,float Weight = 1.0):CharactorObject(FileName,pDevice, ObjectShape, ShapeSize,Weight)
{
	m_pCollider = make_shared<Collider>();
	ShapeSize *= 1.2;
	PhysicsWorld::GetInstance().CreateCollider(m_pCollider, ObjectShape, ColliderShape, m_WorldMat);
	m_pCollider->SetEnterCallBack(&PlayerObject::CallBackFanc, this);
	m_pJoint = make_shared<Joint>();
	m_pActor->Init();
}

PlayerObject::PlayerObject(const char* FileName, ComPtr<ID3D11Device> pDevice,Transform Trans, int ObjectShape, XMVECTOR ShapeSize, XMVECTOR ColliderShape, float Weight = 1.0) :CharactorObject(FileName, pDevice, Trans, ObjectShape, ShapeSize, Weight)
{
	m_pCollider = make_shared<Collider>();
	PhysicsWorld::GetInstance().CreateCollider(m_pCollider, 1, ColliderShape, m_WorldMat);
	m_pCollider->SetEnterCallBack(&PlayerObject::CallBackFanc, this);
	m_pJoint = make_shared<Joint>();
	m_pActor->Init();
}

PlayerObject::~PlayerObject()
{
	Release();
}

void PlayerObject::Init(Transform Trans)
{
	m_pJoint.reset();
	m_pJoint = make_shared<Joint>();
	m_WorldMat.SetMat(Trans.GetMat());
	m_pActor->SetPxMat(Trans);
	m_pCollider->Init();
	m_pCollider->SetPxTrans(Trans);
	I.InputStart();
}

void PlayerObject::Init()
{
	m_pJoint.reset();
	m_pJoint = make_shared<Joint>();
	m_pActor->SetPxMat(m_WorldMat);
	m_pCollider->Init();
	m_pCollider->SetPxTrans(m_WorldMat);
	m_pActor->ClearAllForce();
	I.InputStart();
	
}

void PlayerObject::Update()
{
	if(m_pCollider!=NULL)
		m_pCollider->Update();

	if(m_pActor!=NULL)
	{
		m_pActor->Update();
		m_WorldMat = m_pActor->GetPxTrans();
		m_pCollider->SetPxTrans(m_WorldMat);
	}

	if (I.GetKeyState(DIK_W))
	{
		m_pActor->AddTorque(1.0, Flont->GetFlont());
	}

	if (I.GetKeyState(DIK_S))
	{
		m_pActor->AddTorque(-1.0, Flont->GetFlont());
	}
}

void PlayerObject::SetJoint(PhysXActor* Actor)
{
	PhysicsWorld::GetInstance().CreateJoint(m_pJoint, m_pActor.get(), Actor);
}

void PlayerObject::CallBackFanc(void* Ptr, PhysXActor* pActor)
{
	reinterpret_cast<PlayerObject*>(Ptr)->SetJoint(pActor);
}

void PlayerObject::Release()
{

}

void PlayerObject::CopyObject(PlayerObject* ppObject)
{
	
}