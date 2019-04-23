#include "stdafx.h"
#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	m_ElapsedTime = 60;
}


PhysicsWorld::~PhysicsWorld()
{
}

//当たり判定フィルター
PxFilterFlags FilterShader(
	PxFilterObjectAttributes Attributes0, PxFilterData FilterData0,
	PxFilterObjectAttributes Attributes1, PxFilterData FilterData1,
	PxPairFlags& PairFlgs, const void* ConstantBlock, PxU32 ConstantSize
)
{
	//互いに除外設定されていたら判定しない
	if ((FilterData0.word0&FilterData1.word1) && (FilterData1.word0&FilterData0.word1))
	{
		return PxFilterFlag::eKILL;
	}

	//どちらかがトリガーアクタならトリガーとして判定
	if (PxFilterObjectIsTrigger(Attributes0) || PxFilterObjectIsTrigger(Attributes1))
	{
		PairFlgs = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	PairFlgs = PxPairFlag::eCONTACT_DEFAULT;

	return PxFilterFlag::eDEFAULT;
}

//Physics初期化
void PhysicsWorld::InitPhysics()
{
	//Physics設定
	PxDefaultErrorCallback  Err;
	PxFoundation*			Foundation;

	//Foundation作成　すでに作成済みならNULLとなる
	Foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, m_Allocater, Err);

	//foudationが作られていたか
	if (Foundation == NULL)
	{
		//作られていたらそれを使ってPhysics設定
		Foundation = &PxGetFoundation();
		m_pPhysics = &PxGetPhysics();
	}
	else
	{
		//無かったら作成されたものでPhysicsデバッガの設定
#ifdef _DEBUG
		m_pPvd = PxCreatePvd(*Foundation);
		PxPvdTransport* Transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_pPvd->connect(*Transport, PxPvdInstrumentationFlag::eALL);
		m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation, PxTolerancesScale(), false, m_pPvd);
#else
		m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation, PxTolerancesScale());
#endif
	}

	//当たり判定に関するコールバック
	m_pCallBack = new PhysXCallBack;
	
	CreateScene();

	PxPvdSceneClient* Client = m_pScene->getScenePvdClient();

	if (Client)
	{
		Client->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		Client->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		Client->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	//ジョイント設定
#ifdef _DEBUG
	PxInitExtensions(*m_pPhysics, m_pPvd);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
#endif

	//Cooking設定
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *Foundation, PxCookingParams(m_pPhysics->getTolerancesScale()));
}

//更新時間の設定
void PhysicsWorld::InitPhysics(double ElapsedTime)
{
	InitPhysics();
	m_ElapsedTime = ElapsedTime;
}

//OriginアクタがJointedアクタをその場にジョイントする
void PhysicsWorld::CreateJoint(shared_ptr<Joint> pJoint, PhysXActor* JointActor, PhysXActor* JointedActor)
{
	PxFixedJoint* JointObject;
	PxRigidActor* pActor = JointActor->GetActor();
	PxRigidActor* pJointedActor = JointedActor->GetActor();

	if (pActor == pJointedActor)
		return;

	//力を持ったアクタなら力を0にする
	if (pActor->getType() == PxActorType::eRIGID_DYNAMIC)
	{
		((PxRigidDynamic*)pActor)->clearForce();
		((PxRigidDynamic*)pActor)->clearTorque();
	}

	//力を持ったアクタなら力を0にする
	if (pJointedActor->getType() == PxActorType::eRIGID_DYNAMIC)
	{
		((PxRigidDynamic*)pJointedActor)->clearForce();
		((PxRigidDynamic*)pJointedActor)->clearTorque();
	}

	//ジョイントするオブジェクトの姿勢の逆行列を計算
	PxTransform OriginTrans = pActor->getGlobalPose();
	PxTransform JointedTrans = pJointedActor->getGlobalPose();
	PxQuat OriginQuat = OriginTrans.q;
	XMVECTOR Quat=XMLoadFloat4(&XMFLOAT4(OriginQuat.x, OriginQuat.y, OriginQuat.z, OriginQuat.w));
	XMMATRIX AngleMat = XMMatrixRotationQuaternion(Quat);
	AngleMat=XMMatrixTranspose(AngleMat);
	XMVECTOR JointQuat=XMQuaternionRotationMatrix(AngleMat);
	XMFLOAT4 JointRotation;
	XMStoreFloat4(&JointRotation, JointQuat);

	//ジョイントするオブジェクトから見たジョイントの座標
	PxVec3 OriginPos = pActor->getGlobalPose().p;
	PxVec3 JointedPos = pJointedActor->getGlobalPose().p;
	PxVec3 JointFromOrigin = JointedPos - OriginPos;
	XMMATRIX PosMat=XMMatrixTranslation(JointFromOrigin.x, JointFromOrigin.y, JointFromOrigin.z);
	XMMATRIX OriginMat = PosMat*AngleMat;
	PxVec3 JointPos = PxVec3(OriginMat._41, OriginMat._42, OriginMat._43);

	//ジョイント作成
	PxTransform JointTrans = PxTransform(OriginPos, -OriginQuat);
	JointObject = PxFixedJointCreate(*m_pPhysics, pActor, 
									PxTransform(JointPos,PxQuat(JointRotation.x,JointRotation.y,JointRotation.z,JointRotation.w)), 
									pJointedActor, PxTransform(JointedPos - JointedPos));

#if _DEBUG
	//PhysXデバッガの表示
	JointObject->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, false);
#endif

	pJoint->SetJoint(JointObject);
}

//リジットボディ
void PhysicsWorld::CreateRigid(shared_ptr<RigidBody> pRigidBody, int ObjectShape, XMVECTOR ShapeData, Transform Trans, float Weight=1.0)
{
	switch (ObjectShape)
	{
	case SHAPE::SPHERE:
		float Rad;
		XMStoreFloat(&Rad, ShapeData);
		CreateRigidSphere(pRigidBody, Rad, Trans,Weight);
		break;
	case SHAPE::BOX:
		CreateRigidBox(pRigidBody, ShapeData, Trans, Weight);
		break;
	case SHAPE::CAPSUEL:
		CreateRigidCapsule(pRigidBody, ShapeData, Trans, Weight);
		break;
	default:
		break;
	}
}

//凸メッシュ
void PhysicsWorld::CreateRigidConvex(shared_ptr<RigidBody> ppRigidBody, MyMeshData MeshData, Transform Trans, float Weight = 1.0)
{
	//頂点情報から形状を設定
	vector<PxVec4> Vertics;
	vector<int>    PolyIndex;
	int VertexCount = MeshData.GetVertCount();
	Vertics.resize(VertexCount);
	shared_ptr<MyMesh> pMyMesh = MeshData.GetMesh();

	for (int i = 0; i < VertexCount; i++)
	{
		Vertics[i].x = pMyMesh->m_pVertex->at(i).Position.x;
		Vertics[i].y = pMyMesh->m_pVertex->at(i).Position.y;
		Vertics[i].z = pMyMesh->m_pVertex->at(i).Position.z;
	}

	for (int i = 0; i < MeshData.GetIndexCount(); i++)
	{
		PolyIndex.push_back(MeshData.GetIndex()[i]);
	}

	PxConvexMeshDesc ConvexDesc;
	ConvexDesc.points.count = Vertics.size();
	ConvexDesc.points.stride = sizeof(PxVec4);
	ConvexDesc.points.data = Vertics.data();
	ConvexDesc.polygons.count = PolyIndex.size();
	ConvexDesc.polygons.stride = sizeof(int);
	ConvexDesc.polygons.data = PolyIndex.data();
	ConvexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
	ConvexDesc.vertexLimit = 32;

	PxDefaultMemoryOutputStream Buffer;
	if (m_pCooking->cookConvexMesh(ConvexDesc, Buffer))
	{
		ppRigidBody = NULL;
		return;
	}

	//凸メッシュ作成
	PxDefaultMemoryInputData Input(Buffer.getData(), Buffer.getSize());
	PxConvexMesh* ConvexMesh = m_pPhysics->createConvexMesh(Input);

	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	//位置のキャスト
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	//形状登録
	PxRigidDynamic* Dynamic = PxCreateDynamic(*m_pPhysics, PxTrans, PxConvexMeshGeometry(ConvexMesh), *Material, 1.0f);
	
	Dynamic->userData = ppRigidBody.get();
	ppRigidBody->SetRigidBody(Dynamic);
	m_pScene->addActor(*Dynamic);
	m_pCallBack->m_pRigidBodys.push_back(ppRigidBody.get());
}

//ボックス
void PhysicsWorld::CreateRigidBox(shared_ptr<RigidBody> ppRigidBody, XMVECTOR BoxShape, Transform Trans, float Weight = 1.0)
{
	//PxTransformに変換
	XMFLOAT3 Pos, Shape;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	XMStoreFloat3(&Shape, BoxShape);

	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	//形状登録
	PxRigidDynamic* Dynamic = PxCreateDynamic(*m_pPhysics, PxTrans, PxBoxGeometry(Shape.x, Shape.y, Shape.z), *Material, 1.0f);
	Dynamic->userData = ppRigidBody.get();
	Dynamic->setSolverIterationCounts(10, 10);
	ppRigidBody->SetRigidBody(Dynamic);
	m_pScene->addActor(*Dynamic);
	m_pCallBack->m_pRigidBodys.push_back(ppRigidBody.get());
}

//スフィア
void PhysicsWorld::CreateRigidSphere(shared_ptr<RigidBody> ppRigidBody, float SphereRad, Transform Trans, float Weight = 1.0)
{
	//PxTransformに変換
	XMFLOAT3 Pos, Shape;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	PxRigidDynamic* Dynamic = PxCreateDynamic(*m_pPhysics, PxTrans, PxSphereGeometry(SphereRad), *Material, 1.0f);
	Dynamic->userData = ppRigidBody.get();
	ppRigidBody->SetRigidBody(Dynamic);
	m_pScene->addActor(*Dynamic);
	m_pCallBack->m_pRigidBodys.push_back(ppRigidBody.get());
}

//カプセル
void PhysicsWorld::CreateRigidCapsule(shared_ptr<RigidBody> ppRigidBody, XMVECTOR CapsuleShape, Transform Trans, float Weight = 1.0)
{
	//PxTransformに変換
	XMFLOAT3 Pos;
	XMFLOAT2 Shape;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	XMStoreFloat2(&Shape, CapsuleShape);

	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	PxRigidDynamic* Dynamic = PxCreateDynamic(*m_pPhysics, PxTrans, PxCapsuleGeometry(Shape.x, Shape.y), *Material, 1.0f);
	Dynamic->userData = ppRigidBody.get();
	ppRigidBody->SetRigidBody(Dynamic);
	m_pScene->addActor(*Dynamic);
	m_pCallBack->m_pRigidBodys.push_back(ppRigidBody.get());
}

////スタティック

void PhysicsWorld::CreateStatic(shared_ptr<RigidStatic> pStatic, int ObjectShape, XMVECTOR ShapeData, Transform Trans)
{
	switch (ObjectShape)
	{
	case SHAPE::SPHERE:
		float Rad;
		XMStoreFloat(&Rad, ShapeData);
		CreateStaticSphere(pStatic, Rad, Trans);
		break;
	case SHAPE::BOX:
		CreateStaticBox(pStatic, ShapeData, Trans);
		break;
	case SHAPE::CAPSUEL:
		CreateStaticCapsule(pStatic, ShapeData, Trans);
		break;
	default:
		break;
	}
}

//凸メッシュ
void PhysicsWorld::CreateStaticConvex(shared_ptr<RigidStatic> ppStatic, MyMeshData MeshData, Transform Trans)
{
	//頂点情報から形状を設定
	vector<PxVec4> Vertics;
	vector<int>    PolyIndex;
	int VertexCount=MeshData.GetVertCount();
	Vertics.resize(VertexCount);
	shared_ptr<MyMesh> pMyMesh= MeshData.GetMesh();

	for (int i = 0; i < VertexCount; i++)
	{
		Vertics[i].x = pMyMesh->m_pVertex->at(i).Position.x;
		Vertics[i].y = pMyMesh->m_pVertex->at(i).Position.y;
		Vertics[i].z = pMyMesh->m_pVertex->at(i).Position.z;
	}

	for (int i = 0; i < MeshData.GetIndexCount(); i++)
	{
		PolyIndex.push_back(MeshData.GetIndex()[i]);
	}


	PxConvexMeshDesc ConvexDesc;
	ConvexDesc.points.count = Vertics.size();
	ConvexDesc.points.stride = sizeof(PxVec4);
	ConvexDesc.points.data = Vertics.data();
	ConvexDesc.polygons.count = PolyIndex.size();
	ConvexDesc.polygons.stride = sizeof(int);
	ConvexDesc.polygons.data = PolyIndex.data();
	ConvexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
	ConvexDesc.vertexLimit = 32;

	PxDefaultMemoryOutputStream Buffer;
	if (m_pCooking->cookConvexMesh(ConvexDesc, Buffer))
	{
		ppStatic = NULL;
		return;
	}

	//凸メッシュ作成
	PxDefaultMemoryInputData Input(Buffer.getData(), Buffer.getSize());
	PxConvexMesh* ConvexMesh = m_pPhysics->createConvexMesh(Input);

	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	//PxTransformに変換
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	//形状登録
	PxRigidStatic* Static = PxCreateStatic(*m_pPhysics, PxTrans, PxConvexMeshGeometry(ConvexMesh), *Material);
	Static->userData = ppStatic.get();
	ppStatic->SetActor(Static);
	m_pScene->addActor(*Static);
}

//ボックス
void PhysicsWorld::CreateStaticBox(shared_ptr<RigidStatic> ppStatic, XMVECTOR Shape, Transform Trans)
{
	//PxTransformに変換
	XMFLOAT3 Pos, BoxShape;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	XMStoreFloat3(&BoxShape, Shape);
	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	//形状登録
	PxRigidStatic* Static = PxCreateStatic(*m_pPhysics, PxTrans, PxBoxGeometry(BoxShape.x, BoxShape.y, BoxShape.z), *Material);
	Static->userData = ppStatic.get();
	ppStatic->SetActor(Static);
	m_pScene->addActor(*Static);
}

//スフィア
void PhysicsWorld::CreateStaticSphere(shared_ptr<RigidStatic> ppStatic, float Shape, Transform Trans)
{
	//PxTransformに変換
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);
	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	//形状登録
	PxRigidStatic* Static = PxCreateStatic(*m_pPhysics, PxTrans, PxSphereGeometry(Shape), *Material);
	Static->userData = ppStatic.get();
	ppStatic->SetActor(Static);
	m_pScene->addActor(*Static);
}

//カプセル
void PhysicsWorld::CreateStaticCapsule(shared_ptr<RigidStatic> ppStatic, XMVECTOR Shape, Transform Trans)
{
	//PxTransformに変換
	XMFLOAT3 Pos;
	XMFLOAT2 CapsuleShape;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	XMStoreFloat2(&CapsuleShape, Shape);

	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);
	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	//形状登録
	PxRigidStatic* Static = PxCreateStatic(*m_pPhysics, PxTrans, PxCapsuleGeometry(CapsuleShape.x, CapsuleShape.y), *Material);
	Static->userData = ppStatic.get();
	ppStatic->SetActor(Static);
	m_pScene->addActor(*Static);
}

////トリガーコライダー
void PhysicsWorld::CreateCollider(shared_ptr<Collider> pCollider, int ObjectShape, XMVECTOR ShapeData, Transform Trans)
{
	switch (ObjectShape)
	{
	case SHAPE::SPHERE:
		float Rad;
		XMStoreFloat(&Rad, ShapeData);
		CreateColliderSphere(pCollider, Rad, Trans);
		break;
	case SHAPE::BOX:
		CreateColliderBox(pCollider, ShapeData, Trans);
		break;
	case SHAPE::CAPSUEL:
		CreateColliderCapsule(pCollider, ShapeData, Trans);
		break;
	default:
		break;
	}
}

//凸メッシュ
void PhysicsWorld::CreateColliderConvex(shared_ptr<Collider> ppCollider, MyMeshData MeshData, Transform Trans)
{
	//頂点情報から形状を設定
	vector<PxVec4> Vertics;
	vector<int>    PolyIndex;
	int VertexCount = MeshData.GetVertCount();
	Vertics.resize(VertexCount);
	shared_ptr<MyMesh> pMyMesh = MeshData.GetMesh();

	for (int i = 0; i < VertexCount; i++)
	{
		Vertics[i].x = pMyMesh->m_pVertex->at(i).Position.x;
		Vertics[i].y = pMyMesh->m_pVertex->at(i).Position.y;
		Vertics[i].z = pMyMesh->m_pVertex->at(i).Position.z;
	}

	for (int i = 0; i < MeshData.GetIndexCount(); i++)
	{
		PolyIndex.push_back(MeshData.GetIndex()[i]);
	}

	PxConvexMeshDesc ConvexDesc;
	ConvexDesc.points.count = Vertics.size();
	ConvexDesc.points.stride = sizeof(PxVec4);
	ConvexDesc.points.data = Vertics.data();
	ConvexDesc.polygons.count = PolyIndex.size();
	ConvexDesc.polygons.stride = sizeof(int);
	ConvexDesc.polygons.data = PolyIndex.data();
	ConvexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
	ConvexDesc.vertexLimit = 32;

	PxDefaultMemoryOutputStream Buffer;
	if (m_pCooking->cookConvexMesh(ConvexDesc, Buffer))
	{
		ppCollider = NULL;
		return;
	}

	//凸メッシュ作成
	PxDefaultMemoryInputData Input(Buffer.getData(), Buffer.getSize());
	PxConvexMesh* ConvexMesh = m_pPhysics->createConvexMesh(Input);

	//摩擦
	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	//PxTransformに変換
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	//形状登録
	PxRigidStatic* Static = PxCreateStatic(*m_pPhysics, PxTrans, PxConvexMeshGeometry(ConvexMesh), *Material);
	PxShape* TrgShape;
	Static->getShapes(&TrgShape, 1);
	TrgShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	TrgShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	ppCollider->SetActor(Static);
	Static->userData = ppCollider.get();
	ppCollider->SetActor(Static);
	m_pScene->addActor(*Static);
	m_pCallBack->m_pColliders.push_back(ppCollider.get());
}

//ボックス
void PhysicsWorld::CreateColliderBox(shared_ptr<Collider> ppCollider, XMVECTOR Shape, Transform Trans)
{
	//PxTransformに変換
	XMFLOAT3 Pos, BoxShape;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	XMStoreFloat3(&BoxShape, Shape);

	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	PxRigidStatic* Static = PxCreateStatic(*m_pPhysics, PxTrans, PxBoxGeometry(BoxShape.x, BoxShape.y, BoxShape.z), *Material);
	PxShape* TrgShape;
	Static->getShapes(&TrgShape, 1);
	TrgShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	TrgShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	ppCollider->SetActor(Static);
	Static->userData = ppCollider.get();
	m_pScene->addActor(*Static);
	m_pCallBack->m_pColliders.push_back(ppCollider.get());
}

//スフィア
void PhysicsWorld::CreateColliderSphere(shared_ptr<Collider> ppCollider, float Shape, Transform Trans)
{
	//PxTransformに変換
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos, Trans.GetPosition());

	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	PxRigidStatic* Static = PxCreateStatic(*m_pPhysics, PxTrans, PxSphereGeometry(Shape), *Material);
	PxShape* TrgShape;
	Static->getShapes(&TrgShape, 1);
	TrgShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	TrgShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	ppCollider->SetActor(Static);
	Static->userData = ppCollider.get();
	m_pScene->addActor(*Static);
	m_pCallBack->m_pColliders.push_back(ppCollider.get());
}

//カプセル
void PhysicsWorld::CreateColliderCapsule(shared_ptr<Collider> ppCollider, XMVECTOR Shape, Transform Trans)
{
	//PxTransformに変換
	XMFLOAT3 Pos;
	XMFLOAT2 CapsuleShape;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	XMStoreFloat2(&CapsuleShape, Shape);

	PxVec3 PosVec3 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxTransform PxTrans = PxTransform(PosVec3);

	PxMaterial* Material = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	PxRigidStatic* Static = PxCreateStatic(*m_pPhysics, PxTrans, PxCapsuleGeometry(CapsuleShape.x, CapsuleShape.y), *Material);
	PxShape* TrgShape;
	Static->getShapes(&TrgShape, 1);
	TrgShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	TrgShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	ppCollider->SetActor(Static);
	Static->userData = ppCollider.get();
	m_pScene->addActor(*Static);
	m_pCallBack->m_pColliders.push_back(ppCollider.get());
}

//PhysX更新
void PhysicsWorld::StepPhysics()
{
	m_pScene->simulate(1.0f / 20.0f);
	m_pScene->fetchResults(true);
}

PxScene* PhysicsWorld::GetScene()
{
	return m_pScene;
}

void PhysicsWorld::ClearScene()
{
	m_pCallBack->Clear();
	m_pScene->release();
}

void PhysicsWorld::CreateScene()
{
	//シーン設定
	PxSceneDesc SceneDesc(m_pPhysics->getTolerancesScale());
	SceneDesc.gravity = PxVec3(0, -9.8, 0);
	m_pCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	SceneDesc.cpuDispatcher = m_pCpuDispatcher;
	SceneDesc.filterShader = FilterShader;
	SceneDesc.simulationEventCallback = m_pCallBack;
	m_pScene = m_pPhysics->createScene(SceneDesc);
}

void PhysicsWorld::Release()
{
	if (m_pCpuDispatcher != NULL)
	{
		m_pCpuDispatcher->release();
		m_pCpuDispatcher = NULL;
	}

	if (m_pPhysics != NULL)
	{
		m_pPhysics->release();
		m_pPhysics = NULL;
	}

	if (m_pPvd != NULL)
	{
		m_pPvd->release();
		m_pPvd = NULL;
	}
	
	if (m_pScene != NULL)
	{
		m_pScene->release();
		m_pScene = NULL;
	}
}