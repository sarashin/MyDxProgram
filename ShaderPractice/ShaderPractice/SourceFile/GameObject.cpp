#include "stdafx.h"
#include "GameObject.h"

#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))

//コンストラクタ
GameObject::GameObject()
{
	IsEnable = true;
}

GameObject::GameObject(Transform Trans)
{
	IsEnable = true;
}

//デストラクタ
GameObject::~GameObject()
{
	
}