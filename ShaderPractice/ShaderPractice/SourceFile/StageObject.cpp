#include "stdafx.h"
#include "StageObject.h"

StageObject::StageObject()
{
}

StageObject::StageObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int ObjectShape, XMVECTOR ShapeSize):MeshObject(FileName,pDevice),StaticObject(ObjectShape,ShapeSize,Transform(XMFLOAT3()))
{
	
}

StageObject::StageObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize) :MeshObject(FileName, pDevice), StaticObject(ObjectShape, ShapeSize, Trans)
{
	
}

StageObject::StageObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidStatic> pActor)
{
	m_pMesh = pMesh;
	m_pActor = pActor;
	
	m_WorldMat = m_pActor->GetPxTrans();
}

StageObject::StageObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidStatic> pActor, Transform Trans)
{
	m_pMesh = pMesh;
	m_pActor = pActor;
	
	m_WorldMat = Trans;
}

StageObject::~StageObject()
{
}

void StageObject::Init(XMVECTOR Pos)
{
	m_pActor->SetPxTrans(m_WorldMat);
}

void StageObject::Update()
{}

void StageObject::CopyObject(StageObject** ppObject)
{
	*ppObject = new StageObject(this->m_pMesh, this->m_pActor);
}

void StageObject::Release()
{
	m_pActor.reset();
	m_pMatrixBuffer.Reset();
	m_pMesh.reset();
}