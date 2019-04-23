#pragma once
#include "MeshObject.h"
#include "ColliderObject.h"
#include"PlayerObject.h"

class GoalObject :public MeshObject, public ColliderObject
{
	weak_ptr<PlayerObject> m_pPlayer;
	weak_ptr<SpriteObject> m_pClearGUI;
public:
	bool IsClear;
	GoalObject();
	GoalObject(shared_ptr<PlayerObject> pPlayer,shared_ptr<SpriteObject> pClearSprite);
	~GoalObject();
	bool Update();
	void Release();

};

