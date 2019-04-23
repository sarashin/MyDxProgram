#include"stdafx.h"
#include "GoalObject.h"


GoalObject::GoalObject()
{
	
	IsClear = false;
}

GoalObject::GoalObject(shared_ptr<PlayerObject> pPlayer, shared_ptr<SpriteObject> pClearSprite)
{

	IsClear = false;
}

GoalObject::~GoalObject()
{

}

bool GoalObject::Update()
{
	
	return false;
}

void GoalObject::Release()
{

}