#pragma once
#include "MeshObject.h"
#include"ColliderObject.h"

class AreaObject : public MeshObject, public ColliderObject
{
public:
	//�R���X�g���N�^
	AreaObject();

	//�R���X�g���N�^
	//FileName  : �t�@�C����
	//pDevice   : �f�o�C�X
	//Trans     : �������W
	//Shape     : PhysX��̌`��
	//ShapeSize : �`��̑傫��
	AreaObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int ObjectShape, XMVECTOR ShapeSize);

	//�R���X�g���N�^
	//FileName  : �t�@�C����
	//pDevice   : �f�o�C�X
	//Shape     : PhysX�̌`��
	//ShapeSize : �`��̑傫��
	AreaObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize);

	//�R���X�g���N�^
	//pMesh : ���b�V���̃|�C���^
	//pActor: PhysX�̃A�N�^�̃|�C���^
	AreaObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<Collider> pActor);

	//�R���X�g���N�^
	//pMesh : ���b�V���̃|�C���^
	//pActor: PhysX�̃A�N�^�̃|�C���^
	//Trans : �������W
	AreaObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<Collider> pActor, Transform Trans);

	//�f�X�g���N�^
	~AreaObject();

	//������
	//Pos : ���W
	void Init(XMVECTOR Pos);
	//�X�V
	void Update();
	//�R�s�[
	void CopyObject(AreaObject** Object);
	//���
	void Release()override;
};

