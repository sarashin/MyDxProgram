#pragma once
#include"RigidBody.h"
#include"RigidStatic.h"
#include"Joint.h"
#include"PhysXCallBack.h"
#include"Mesh.h"

using namespace physx;
//PhysXクラス
class PhysicsWorld : public Singleton<PhysicsWorld>
{
	//アロケーター
	PxDefaultAllocator				m_Allocater;			
	//PhysX
	PxPhysics*				m_pPhysics = NULL;	
	PxDefaultCpuDispatcher*	m_pCpuDispatcher = NULL;
	//PhysXシ－ン
	PxScene*					m_pScene = NULL;	
	//おそらくGPU関係
	PxPvd*					m_pPvd = NULL;			
	//メッシュファクトリ
	PxCooking*				m_pCooking;				

	//接触コールバック
	PhysXCallBack*					m_pCallBack;		
	//更新時間
	double							m_ElapsedTime;			
public:
	//コンストラクタ
	PhysicsWorld();
	//デストラクタ
	~PhysicsWorld();

	//初期化
	void InitPhysics();
	//初期化
	//ElapsedTime:更新頻度(フレーム毎)
	void InitPhysics(double ElapsedTime);
	
	//物理オブジェクト作成
	//Static
	void CreateStaticConvex(shared_ptr<RigidStatic> ppStatic, MyMeshData MeshData, Transform Trans);
	void CreateStaticBox(shared_ptr<RigidStatic> ppStatic, XMVECTOR Shape, Transform Trans);
	void CreateStaticSphere(shared_ptr<RigidStatic> ppStatic, float Shape, Transform Trans);
	void CreateStaticCapsule(shared_ptr<RigidStatic> ppStatic, XMVECTOR Shape, Transform Trans);
	void CreateStatic(shared_ptr<RigidStatic> ppStatic, int ObjectShape, XMVECTOR ShapeData, Transform Trans);

	//リジットボディ
	void CreateRigidConvex(shared_ptr<RigidBody> ppRigidBody, MyMeshData MeshData, Transform Trans, float Weight);
	void CreateRigidBox(shared_ptr<RigidBody> ppRigidBody, XMVECTOR Shape, Transform Trans, float Weight);
	void CreateRigidSphere(shared_ptr<RigidBody> ppRigidBody, float Shape, Transform Trans, float Weight);
	void CreateRigidCapsule(shared_ptr<RigidBody> ppRigidBody, XMVECTOR Shape, Transform Trans, float Weight);
	void CreateRigid(shared_ptr<RigidBody> pRigidBody, int ObjectShape, XMVECTOR ShapeData, Transform Trans, float Weight);

	//トリガーコライダ
	void CreateColliderConvex(shared_ptr<Collider> ppColider, MyMeshData MeshData, Transform Trans);
	void CreateColliderBox(shared_ptr<Collider> ppCollider, XMVECTOR Shape, Transform Trans);
	void CreateColliderSphere(shared_ptr<Collider> ppCollider, float Shape, Transform Trans);
	void CreateColliderCapsule(shared_ptr<Collider> ppCollider, XMVECTOR Shape, Transform Trans);
	void CreateCollider(shared_ptr<Collider> pRigidBody, int ObjectShape, XMVECTOR ShapeData, Transform Trans);

	//ジョイント
	void CreateJoint(shared_ptr<Joint> pJoint, PhysXActor* Joint, PhysXActor* Jointed);
	//シーンの取得
	PxScene* GetScene();

	//PhysX更新
	void ClearScene();
	void CreateScene();
	void StepPhysics();
	//解放
	void Release();
};

