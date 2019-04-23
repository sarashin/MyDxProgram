#pragma once
#include"PhysXActor.h"

class StageData
{
public:
	
	//���f���̃p�X
	vector<char*>			Models;		
	//���Ԗڂ̃��f����
	vector<int>				ModelNum;	
	//�����ʒu
	vector<XMMATRIX>		Transforms;	
	//RigidActor�̎��
	vector<int>				Actors;		
	//�`��
	vector<int>				Shapes;		
	//�`��̑傫��
	vector<XMFLOAT3>		ShapeDatas;	
	//�d��
	vector<float>			Weight;
	//���V�[��
	DWORD					NextScene;

	StageData();
	~StageData();

	void First();
	void Second();
	void Thard();
};