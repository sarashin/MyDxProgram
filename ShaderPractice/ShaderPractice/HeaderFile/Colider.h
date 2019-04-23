#pragma once
#include"PhysXActor.h"

typedef void (*FUNC_POINTER)(void* Ptr, PhysXActor* pActor);

class Collider : public PhysXActor
{
protected:
	//PhysXコライダーのポインタ
	PxRigidStatic* m_pCollider;	
	//コライダーを所持しているクラスのポインタ
	void* pObject;
	//コライダーに触った時呼ばれる関数
	FUNC_POINTER m_pEnterFunc;
	//コライダーから離れた時に呼ばれる関数
	FUNC_POINTER m_pExitFunc;
	//他オブジェクトとのフィルタ
	void SetUpFilterGroup(PxRigidActor* Actor, PxU32 FilterGroup, PxU32 FilterMask);
public:
	//接触してるオブジェクト
	vector<PhysXActor*> Contact;
	//接触していたオブジェクト
	vector<PhysXActor*> Contacted;
	//コライダーに接触したオブジェクト
	vector<PhysXActor*> Enter;

	//コンストラクタ
	Collider();
	//コンストラクタ
	//pActor:アクタのポインタ
	Collider(PxRigidStatic* pActor);
	//デストラクタ
	~Collider();

	//初期化
	void Init();
	//更新
	void Update();
	//アクタの設定
	void SetActor(PxRigidStatic* Collider);
	//アクタのポインタの取得
	PxRigidActor* GetActor();
	//PhysX上のポインタの取得
	Transform GetPxTrans();
	//位置の設定
	void SetPxTrans(Transform Trans);
	//コールバック関数の設定
	//ExitCall,EnterCall: コールバック関数
	//Class				: コールバック関数を所持するクラス
	void SetEnterCallBack(FUNC_POINTER EnterCall,void* Class);
	void SetExitCallBack(FUNC_POINTER EnterCall, void* Class);
	//PhysXで呼ばれるコールバック関数
	//pTarget:当たったオブジェクトのポインタ
	void OnCollisionEnter(PhysXActor* pTarget);
	void OnCollisionExit(PhysXActor* pTarget);
	//解放
	void Release() override;
};