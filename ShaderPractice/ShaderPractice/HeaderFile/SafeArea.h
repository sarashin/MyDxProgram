#pragma once
#include "ColliderObject.h"
#include"PlayerObject.h"

class SafeArea :public ColliderObject
{
	weak_ptr<PlayerObject> m_pPlayer;
	bool IsEnter;
public:
	//�R���X�g���N�^
	SafeArea();
	//�R���X�g���N�^
	//pActor:�A�N�^�̃|�C���^
	SafeArea(int Shape, XMVECTOR Size, Transform Trans,shared_ptr<PlayerObject> pPlayer);
	//�f�X�g���N�^
	~SafeArea();

	//������
	void Init();
	//PhysX�ɓn���R�[���o�b�N�֐�
	//Ptr:this�|�C���^
	//pActor:�ڐG�����A�N�^�̃|�C���^
	static void CallBackFunc(void* Ptr, PhysXActor* pActor);

	//�I�u�W�F�N�g�̈ʒu���Đݒ�
	//pActor:�Đݒ肷��A�N�^�̃|�C���^
	void DeleteObject(PhysXActor* Actor);

	//�X�V
	bool Update();

	//���
	void Release();
};

