#pragma once
#include"Transform.h"
#include"ActorObject.h"
using namespace physx;

//PhysXのフィルタに使う構造体
struct FilterGroup
{
	enum Filter
	{
		eSPHERE = (1 << 0),
		eBOX = (1 << 1),
		eCAPSUEL = (1 << 2),
		eCONVEX = (1 << 3),
		eLAND = (1 << 4),
	};
};

//形状のenum
enum SHAPE
{
	BOX=0,
	SPHERE,
	CAPSUEL,
	CONVEX,
	TRI,
};

//PhysXのアクタ
class PhysXActor
{
protected:
	//フィルタ
	//pActor:アクタのポインタ
	//FilterGroup:属するフィルタ
	//FilterMask:どれをフィルタするか
	virtual void SetUpFilterGroup(physx::PxRigidActor* pActor, physx::PxU32 FilterGroup, physx::PxU32 FilterMask)=0;
	
public:
	//初期化
	virtual void		Init() = 0;
	//更新
	virtual void		Update()=0;
	//PhysX上の座標取得
	virtual Transform	GetPxTrans()=0;
	//アクタの取得
	virtual PxRigidActor* GetActor()=0;
	//解放
	virtual void		Release()=0;
};

