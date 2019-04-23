#pragma once
#include "RigidObject.h"
#include "Mesh.h"
#include "MeshObject.h"
#include "Joint.h"

class CharactorObject :public MeshObject,public RigidObject
{
protected:
	
public:
	//�R���X�g���N�^
	CharactorObject();

	//�R���X�g���N�^
	//FileName  : �t�@�C����
	//pDevice   : �`��f�o�C�X
	//Shape �@�@: PhysX��̌`��
	//ShapeSize : �`��̑傫��
	//Weight    : ����
	CharactorObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int Shape, XMVECTOR ShapeSize, float Weight);

	//�R���X�g���N�^
	//FileName  : �t�@�C����
	//pDevice   : �`��f�o�C�X
	//Transform : �������W
	//Shape �@�@: PhysX��̌`��
	//ShapeSize : �`��̑傫��
	//Weight    : ����
	CharactorObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize, float Weight);

	//�R���X�g���N�^
	//pMesh : ���b�V���̃|�C���^
	//pActor: �R�s�[����PhysX�̃A�N�^
	CharactorObject(shared_ptr<vector<MyMeshData>> pMesh, RigidBody pActor);

	//�R���X�g���N�^
	//pMesh : ���b�V���̃|�C���^
	//pActor: �R�s�[����PhysX�̃A�N�^
	//Trans : �������W
	CharactorObject(shared_ptr<vector<MyMeshData>> pMesh, RigidBody pActor,Transform Trans);

	//�f�X�g���N�^
	~CharactorObject();
	
	void Init();
	void Init(XMVECTOR Pos);
	void SetMat(CXMMATRIX Pos);
	void Update();

	void CopyObject(CharactorObject* Object);
	void CopyObject(unique_ptr<CharactorObject>& ppObject,Transform Trans);
	
	void Release()override;
};

