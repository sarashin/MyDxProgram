#pragma once
#include"RigidBody.h"
#include"RigidStatic.h"
#include"Joint.h"
#include"PhysXCallBack.h"
#include"Mesh.h"

using namespace physx;
//PhysX�N���X
class PhysicsWorld : public Singleton<PhysicsWorld>
{
	//�A���P�[�^�[
	PxDefaultAllocator				m_Allocater;			
	//PhysX
	PxPhysics*				m_pPhysics = NULL;	
	PxDefaultCpuDispatcher*	m_pCpuDispatcher = NULL;
	//PhysX�V�|��
	PxScene*					m_pScene = NULL;	
	//�����炭GPU�֌W
	PxPvd*					m_pPvd = NULL;			
	//���b�V���t�@�N�g��
	PxCooking*				m_pCooking;				

	//�ڐG�R�[���o�b�N
	PhysXCallBack*					m_pCallBack;		
	//�X�V����
	double							m_ElapsedTime;			
public:
	//�R���X�g���N�^
	PhysicsWorld();
	//�f�X�g���N�^
	~PhysicsWorld();

	//������
	void InitPhysics();
	//������
	//ElapsedTime:�X�V�p�x(�t���[����)
	void InitPhysics(double ElapsedTime);
	
	//�����I�u�W�F�N�g�쐬
	//Static
	void CreateStaticConvex(shared_ptr<RigidStatic> ppStatic, MyMeshData MeshData, Transform Trans);
	void CreateStaticBox(shared_ptr<RigidStatic> ppStatic, XMVECTOR Shape, Transform Trans);
	void CreateStaticSphere(shared_ptr<RigidStatic> ppStatic, float Shape, Transform Trans);
	void CreateStaticCapsule(shared_ptr<RigidStatic> ppStatic, XMVECTOR Shape, Transform Trans);
	void CreateStatic(shared_ptr<RigidStatic> ppStatic, int ObjectShape, XMVECTOR ShapeData, Transform Trans);

	//���W�b�g�{�f�B
	void CreateRigidConvex(shared_ptr<RigidBody> ppRigidBody, MyMeshData MeshData, Transform Trans, float Weight);
	void CreateRigidBox(shared_ptr<RigidBody> ppRigidBody, XMVECTOR Shape, Transform Trans, float Weight);
	void CreateRigidSphere(shared_ptr<RigidBody> ppRigidBody, float Shape, Transform Trans, float Weight);
	void CreateRigidCapsule(shared_ptr<RigidBody> ppRigidBody, XMVECTOR Shape, Transform Trans, float Weight);
	void CreateRigid(shared_ptr<RigidBody> pRigidBody, int ObjectShape, XMVECTOR ShapeData, Transform Trans, float Weight);

	//�g���K�[�R���C�_
	void CreateColliderConvex(shared_ptr<Collider> ppColider, MyMeshData MeshData, Transform Trans);
	void CreateColliderBox(shared_ptr<Collider> ppCollider, XMVECTOR Shape, Transform Trans);
	void CreateColliderSphere(shared_ptr<Collider> ppCollider, float Shape, Transform Trans);
	void CreateColliderCapsule(shared_ptr<Collider> ppCollider, XMVECTOR Shape, Transform Trans);
	void CreateCollider(shared_ptr<Collider> pRigidBody, int ObjectShape, XMVECTOR ShapeData, Transform Trans);

	//�W���C���g
	void CreateJoint(shared_ptr<Joint> pJoint, PhysXActor* Joint, PhysXActor* Jointed);
	//�V�[���̎擾
	PxScene* GetScene();

	//PhysX�X�V
	void ClearScene();
	void CreateScene();
	void StepPhysics();
	//���
	void Release();
};

