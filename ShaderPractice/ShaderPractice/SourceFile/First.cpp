#include "stdafx.h"
#include "First.h"

First::First()
{
	Models.push_back("../PlayerBox.fbx");
	Models.push_back("../BlueBox2.fbx");
	Models.push_back("../floor.fbx");
	ModelNum.push_back(1);
	ModelNum.push_back(1);
	ModelNum.push_back(1);
	ModelNum.push_back(2);
	ModelNum.push_back(2);
	ModelNum.push_back(2);
	ModelNum.push_back(2);
	Transforms.push_back(XMMatrixTranslation(-50.0f, 60.0f, 0.0f));
	Transforms.push_back(XMMatrixTranslation(-80.0f, 80.0f, 0.0f));
	Transforms.push_back(XMMatrixTranslation(50.0f, 40.0f, 30.0f));
	Transforms.push_back(XMMatrixTranslation(90.0f, 30.0f, 0.0f));
	Transforms.push_back(XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	Transforms.push_back(XMMatrixTranslation(0.0f, 20.0f, 200.0f));
	Transforms.push_back(XMMatrixTranslation(200.0f, 40.0f, 200.0f));
	Transforms.push_back(XMMatrixTranslation(200.0f, 60.0f, 0.0f));
	Transforms.push_back(XMMatrixTranslation(200.0f, 115.0f, 0.0f));
	/*Actors.push_back(ACTOR::RIGID_BODY);
	Actors.push_back(ACTOR::RIGID_BODY);
	Actors.push_back(ACTOR::RIGID_BODY);
	Actors.push_back(ACTOR::STATIC);
	Actors.push_back(ACTOR::STATIC);
	Actors.push_back(ACTOR::STATIC);
	Actors.push_back(ACTOR::STATIC);*/
	Shapes.push_back(SHAPE::BOX);
	Shapes.push_back(SHAPE::BOX);
	Shapes.push_back(SHAPE::BOX);
	Shapes.push_back(SHAPE::BOX);
	Shapes.push_back(SHAPE::BOX);
	Shapes.push_back(SHAPE::BOX);
	Shapes.push_back(SHAPE::BOX);
	ShapeDatas.push_back(XMFLOAT3(10.0f, 10.0f, 10.0f));
	ShapeDatas.push_back(XMFLOAT3(10.0f, 10.0f, 10.0f));
	ShapeDatas.push_back(XMFLOAT3(10.0f, 10.0f, 10.0f));
	ShapeDatas.push_back(XMFLOAT3(100.0, 10.0, 100.0));
	ShapeDatas.push_back(XMFLOAT3(100.0, 10.0, 100.0));
	ShapeDatas.push_back(XMFLOAT3(100.0, 10.0, 100.0));
	ShapeDatas.push_back(XMFLOAT3(100.0, 10.0, 100.0));
}


First::~First()
{
}
