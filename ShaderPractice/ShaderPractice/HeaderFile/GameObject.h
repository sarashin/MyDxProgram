#pragma once
#include"Transform.h"

//�Q�[���I�u�W�F�N�g�N���X
class GameObject
{
public:
	//�R���X�g���N�^
	GameObject();
	//�R���X�g���N�^
	//Trans: ���W
	GameObject(Transform Trans);
	//�f�X�g���N�^
	~GameObject();
	//�L�����ǂ���
	bool						IsEnable;			
	//���
	virtual void Release() = 0;						
};
