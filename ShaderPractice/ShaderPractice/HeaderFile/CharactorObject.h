#pragma once
#include "RigidObject.h"
#include "Mesh.h"
#include "MeshObject.h"
#include "Joint.h"

class CharactorObject :public MeshObject,public RigidObject
{
protected:
	
public:
	//コンストラクタ
	CharactorObject();

	//コンストラクタ
	//FileName  : ファイル名
	//pDevice   : 描画デバイス
	//Shape 　　: PhysX上の形状
	//ShapeSize : 形状の大きさ
	//Weight    : 質量
	CharactorObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int Shape, XMVECTOR ShapeSize, float Weight);

	//コンストラクタ
	//FileName  : ファイル名
	//pDevice   : 描画デバイス
	//Transform : 初期座標
	//Shape 　　: PhysX上の形状
	//ShapeSize : 形状の大きさ
	//Weight    : 質量
	CharactorObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize, float Weight);

	//コンストラクタ
	//pMesh : メッシュのポインタ
	//pActor: コピーするPhysXのアクタ
	CharactorObject(shared_ptr<vector<MyMeshData>> pMesh, RigidBody pActor);

	//コンストラクタ
	//pMesh : メッシュのポインタ
	//pActor: コピーするPhysXのアクタ
	//Trans : 初期座標
	CharactorObject(shared_ptr<vector<MyMeshData>> pMesh, RigidBody pActor,Transform Trans);

	//デストラクタ
	~CharactorObject();
	
	void Init();
	void Init(XMVECTOR Pos);
	void SetMat(CXMMATRIX Pos);
	void Update();

	void CopyObject(CharactorObject* Object);
	void CopyObject(unique_ptr<CharactorObject>& ppObject,Transform Trans);
	
	void Release()override;
};

