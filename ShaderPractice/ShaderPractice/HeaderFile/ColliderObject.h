#pragma once
#include "ActorObject.h"
#include "Colider.h"

class ColliderObject :public ActorObject
{
protected:

public:
	//�A�N�^�̃|�C���^
	shared_ptr<Collider> m_pActor;

	//�R���X�g���N�^
	ColliderObject();
	//�R���X�g���N�^
	//pCollider : �R���C�_�[�̃|�C���^
	ColliderObject(Collider* pCollider);
	//�R���X�g���N�^
	//Shape: �`��
	//Size:�`��̑傫��
	ColliderObject(int Shape, XMVECTOR Size, Transform Trans);
	//�f�X�g���N�^
	~ColliderObject();

	//�A�N�^�̍쐬
	void CreateActor(int ObjectShape, XMVECTOR ShapeSize, Transform Trans);
	//�X�V
	void Update();
	//�R���C�_�̎擾
	Collider*  GetCollider();
	//PhysX��̍��W
	Transform GetPxTrans();
	//���
	void Release()override;
};

