#include "stdafx.h"
#include "AreaObject.h"

AreaObject::AreaObject()
{
}

//�R���X�g���N�^
AreaObject::AreaObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int ObjectShape, XMVECTOR ShapeSize):MeshObject(FileName,pDevice),ColliderObject(ObjectShape,ShapeSize,Transform())
{
	
}

//�R���X�g���N�^
AreaObject::AreaObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int Shape, XMVECTOR ShapeSize) :MeshObject(FileName, pDevice), ColliderObject(Shape, ShapeSize, Trans)
{

}

//�R���X�g���N�^
AreaObject::AreaObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<Collider> pActor)
{
	m_pMesh = pMesh;
	m_pActor = pActor;
	m_WorldMat = m_pActor->GetPxTrans();
}

//�R���X�g���N�^
AreaObject::AreaObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<Collider> pActor, Transform Trans)
{
}

//�f�X�g���N�^
AreaObject::~AreaObject()
{
}

//������
void AreaObject::Init(XMVECTOR Pos)
{
	m_pActor->SetPxTrans(m_WorldMat);
}

//�X�V
void AreaObject::Update()
{}

//�R�s�[
void AreaObject::CopyObject(AreaObject** ppObject)
{}

//���
void AreaObject::Release()
{}