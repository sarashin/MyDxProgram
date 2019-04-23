#pragma once
#include "ColliderObject.h"
#include"PlayerObject.h"

class SafeArea :public ColliderObject
{
	weak_ptr<PlayerObject> m_pPlayer;
	bool IsEnter;
public:
	//コンストラクタ
	SafeArea();
	//コンストラクタ
	//pActor:アクタのポインタ
	SafeArea(int Shape, XMVECTOR Size, Transform Trans,shared_ptr<PlayerObject> pPlayer);
	//デストラクタ
	~SafeArea();

	//初期化
	void Init();
	//PhysXに渡すコールバック関数
	//Ptr:thisポインタ
	//pActor:接触したアクタのポインタ
	static void CallBackFunc(void* Ptr, PhysXActor* pActor);

	//オブジェクトの位置を再設定
	//pActor:再設定するアクタのポインタ
	void DeleteObject(PhysXActor* Actor);

	//更新
	bool Update();

	//解放
	void Release();
};

