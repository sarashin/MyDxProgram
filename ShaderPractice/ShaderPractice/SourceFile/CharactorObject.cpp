#include"stdafx.h"
#include "CharactorObject.h"
#include"PhysicsWorld.h"

#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))


//コンストラクタ
CharactorObject::CharactorObject() : MeshObject()
{
	
}

//コンストラクタ
CharactorObject::CharactorObject(shared_ptr<vector<MyMeshData>> pMesh, RigidBody pActor)
{
	//メッシュのポインタを取得
	m_pMesh = pMesh;

	//Physicsを再設定
	int Shape;
	XMVECTOR Geometry;
	Transform Trans = Transform();

	Shape = pActor.GetShape(Geometry);
	PhysicsWorld::GetInstance().CreateRigid(m_pActor, Shape, Geometry, Trans, pActor.GetWeight());

	//姿勢のコピー
	m_WorldMat = Trans;
	m_InitMat = Trans;
}

//コンストラクタ
CharactorObject::CharactorObject(shared_ptr<vector<MyMeshData>> pMesh, RigidBody pActor,Transform Trans)
{
	//メッシュのポインタを取得
	m_pMesh = pMesh;

	//Physicsを再設定
	int Shape;
	XMVECTOR Geometry;

	Shape = pActor.GetShape(Geometry);
	PhysicsWorld::GetInstance().CreateRigid(m_pActor, Shape, Geometry, Trans,pActor.GetWeight());
	
	//姿勢のコピー
	m_WorldMat = Trans;
	m_InitMat = Trans;
}

//コンストラクタ
CharactorObject::CharactorObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int ObjectShape, XMVECTOR ShapeSize, float Weight = 1.0) : MeshObject(FileName, pDevice),RigidObject(ObjectShape, ShapeSize,Transform(XMFLOAT3(0.0,0.0,0.0)), Weight)
{
	m_WorldMat = Transform();
}

//コンストラクタ
CharactorObject::CharactorObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize, float Weight = 1.0) : MeshObject(FileName,pDevice,Trans),RigidObject(ObjectShape,ShapeSize,Trans,Weight)
{
	m_InitMat = m_pActor->GetPxTrans();
	Init();
}

//デストラクタ
CharactorObject::~CharactorObject()
{
	
}

//初期化
void CharactorObject::Init(XMVECTOR Pos)
{
	//Positionからトランスフォーム作成
	m_WorldMat.SetPosition(Pos);
	Transform Trans = Transform(XMMatrixTranslationFromVector(Pos));
	//アクタに座標を設定
	m_pActor->SetPxMat(Trans);
	m_pActor->ClearAllForce();
}

void CharactorObject::Init()
{
	//アクタに座標を設定
	m_WorldMat = m_InitMat;
	m_pActor->SetPxMat(m_WorldMat);
	m_pActor->ClearAllForce();
}

//座標の設定
void CharactorObject::SetMat(CXMMATRIX Mat)
{
	m_WorldMat.SetMat(Mat);

	//PhysXの座標も一緒に変える
	if(m_pActor!=NULL)
		m_pActor->SetPxMat(this->m_WorldMat);
}

//更新
void CharactorObject::Update()
{
	//アクタがあるならPhysXの座標と同期をとる
	if (m_pActor != NULL)
	{
		m_pActor->Update();
		m_WorldMat = m_pActor->GetPxTrans();
	}
}

//オブジェクトのコピー
void CharactorObject::CopyObject(CharactorObject* ppObject)
{
	ppObject = new CharactorObject(this->m_pMesh,*this->m_pActor);
}

void CharactorObject::CopyObject(unique_ptr<CharactorObject>& ppObject,Transform Trans)
{
	RigidBody Actor = *this->m_pActor.get();
	*ppObject =  CharactorObject(this->m_pMesh, Actor,Trans);
	ppObject->m_pMatrixBuffer = this->m_pMatrixBuffer;
}

//解放
void CharactorObject::Release()
{
	m_pActor->Release();
	m_pActor.reset();
	m_pMatrixBuffer.Reset();
	m_pMesh.reset();
}