#include "stdafx.h"
#include "AreaObject.h"

AreaObject::AreaObject()
{
}

//コンストラクタ
AreaObject::AreaObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int ObjectShape, XMVECTOR ShapeSize):MeshObject(FileName,pDevice),ColliderObject(ObjectShape,ShapeSize,Transform())
{
	
}

//コンストラクタ
AreaObject::AreaObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int Shape, XMVECTOR ShapeSize) :MeshObject(FileName, pDevice), ColliderObject(Shape, ShapeSize, Trans)
{

}

//コンストラクタ
AreaObject::AreaObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<Collider> pActor)
{
	m_pMesh = pMesh;
	m_pActor = pActor;
	m_WorldMat = m_pActor->GetPxTrans();
}

//コンストラクタ
AreaObject::AreaObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<Collider> pActor, Transform Trans)
{
}

//デストラクタ
AreaObject::~AreaObject()
{
}

//初期化
void AreaObject::Init(XMVECTOR Pos)
{
	m_pActor->SetPxTrans(m_WorldMat);
}

//更新
void AreaObject::Update()
{}

//コピー
void AreaObject::CopyObject(AreaObject** ppObject)
{}

//解放
void AreaObject::Release()
{}