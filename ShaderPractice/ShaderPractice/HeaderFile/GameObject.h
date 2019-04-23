#pragma once
#include"Transform.h"

//ゲームオブジェクトクラス
class GameObject
{
public:
	//コンストラクタ
	GameObject();
	//コンストラクタ
	//Trans: 座標
	GameObject(Transform Trans);
	//デストラクタ
	~GameObject();
	//有効かどうか
	bool						IsEnable;			
	//解放
	virtual void Release() = 0;						
};
