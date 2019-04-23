#pragma once
#include"Transform.h"

//アクタのベースクラス
class ActorObject
{
protected:
	Transform m_InitMat;
public:
	//PhysX上の座標取得
	virtual Transform	GetPxTrans() = 0;
	//解放
	virtual void		Release() = 0;


};