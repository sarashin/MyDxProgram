#pragma once
#include "ActorObject.h"
#include "Colider.h"

class ColliderObject :public ActorObject
{
protected:

public:
	//アクタのポインタ
	shared_ptr<Collider> m_pActor;

	//コンストラクタ
	ColliderObject();
	//コンストラクタ
	//pCollider : コライダーのポインタ
	ColliderObject(Collider* pCollider);
	//コンストラクタ
	//Shape: 形状
	//Size:形状の大きさ
	ColliderObject(int Shape, XMVECTOR Size, Transform Trans);
	//デストラクタ
	~ColliderObject();

	//アクタの作成
	void CreateActor(int ObjectShape, XMVECTOR ShapeSize, Transform Trans);
	//更新
	void Update();
	//コライダの取得
	Collider*  GetCollider();
	//PhysX上の座標
	Transform GetPxTrans();
	//解放
	void Release()override;
};

