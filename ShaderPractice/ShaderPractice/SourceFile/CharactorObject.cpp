#include"stdafx.h"
#include "CharactorObject.h"
#include"PhysicsWorld.h"

#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))


//�R���X�g���N�^
CharactorObject::CharactorObject() : MeshObject()
{
	
}

//�R���X�g���N�^
CharactorObject::CharactorObject(shared_ptr<vector<MyMeshData>> pMesh, RigidBody pActor)
{
	//���b�V���̃|�C���^���擾
	m_pMesh = pMesh;

	//Physics���Đݒ�
	int Shape;
	XMVECTOR Geometry;
	Transform Trans = Transform();

	Shape = pActor.GetShape(Geometry);
	PhysicsWorld::GetInstance().CreateRigid(m_pActor, Shape, Geometry, Trans, pActor.GetWeight());

	//�p���̃R�s�[
	m_WorldMat = Trans;
	m_InitMat = Trans;
}

//�R���X�g���N�^
CharactorObject::CharactorObject(shared_ptr<vector<MyMeshData>> pMesh, RigidBody pActor,Transform Trans)
{
	//���b�V���̃|�C���^���擾
	m_pMesh = pMesh;

	//Physics���Đݒ�
	int Shape;
	XMVECTOR Geometry;

	Shape = pActor.GetShape(Geometry);
	PhysicsWorld::GetInstance().CreateRigid(m_pActor, Shape, Geometry, Trans,pActor.GetWeight());
	
	//�p���̃R�s�[
	m_WorldMat = Trans;
	m_InitMat = Trans;
}

//�R���X�g���N�^
CharactorObject::CharactorObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int ObjectShape, XMVECTOR ShapeSize, float Weight = 1.0) : MeshObject(FileName, pDevice),RigidObject(ObjectShape, ShapeSize,Transform(XMFLOAT3(0.0,0.0,0.0)), Weight)
{
	m_WorldMat = Transform();
}

//�R���X�g���N�^
CharactorObject::CharactorObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize, float Weight = 1.0) : MeshObject(FileName,pDevice,Trans),RigidObject(ObjectShape,ShapeSize,Trans,Weight)
{
	m_InitMat = m_pActor->GetPxTrans();
	Init();
}

//�f�X�g���N�^
CharactorObject::~CharactorObject()
{
	
}

//������
void CharactorObject::Init(XMVECTOR Pos)
{
	//Position����g�����X�t�H�[���쐬
	m_WorldMat.SetPosition(Pos);
	Transform Trans = Transform(XMMatrixTranslationFromVector(Pos));
	//�A�N�^�ɍ��W��ݒ�
	m_pActor->SetPxMat(Trans);
	m_pActor->ClearAllForce();
}

void CharactorObject::Init()
{
	//�A�N�^�ɍ��W��ݒ�
	m_WorldMat = m_InitMat;
	m_pActor->SetPxMat(m_WorldMat);
	m_pActor->ClearAllForce();
}

//���W�̐ݒ�
void CharactorObject::SetMat(CXMMATRIX Mat)
{
	m_WorldMat.SetMat(Mat);

	//PhysX�̍��W���ꏏ�ɕς���
	if(m_pActor!=NULL)
		m_pActor->SetPxMat(this->m_WorldMat);
}

//�X�V
void CharactorObject::Update()
{
	//�A�N�^������Ȃ�PhysX�̍��W�Ɠ������Ƃ�
	if (m_pActor != NULL)
	{
		m_pActor->Update();
		m_WorldMat = m_pActor->GetPxTrans();
	}
}

//�I�u�W�F�N�g�̃R�s�[
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

//���
void CharactorObject::Release()
{
	m_pActor->Release();
	m_pActor.reset();
	m_pMatrixBuffer.Reset();
	m_pMesh.reset();
}