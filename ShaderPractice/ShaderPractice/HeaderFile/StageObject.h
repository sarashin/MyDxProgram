#pragma once
#include "MeshObject.h"
#include"StaticObject.h"

class StageObject :public MeshObject,public StaticObject
{
	
public:
	//�R���X�g���N�^
	StageObject();

	//�R���X�g���N�^
	//FileName:�t�@�C����
	//pDevice:�f�o�C�X�̃|�C���^
	//Shape:�`��
	//Size:�`��̑傫��
	StageObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int Shape, XMVECTOR Size);

	//�R���X�g���N�^
	//FileName:�t�@�C����
	//pDevice:�f�o�C�X�̃|�C���^
	//Trans:�������W
	//Shape:�`��
	//Size:�`��̑傫��
	StageObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize);

	//�R���X�g���N�^
	//pMesh:���b�V���̃|�C���^
	//pActor:�A�N�^�̃|�C���^
	StageObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidStatic> pActor);

	//�R���X�g���N�^
	//pMesh:���b�V���̃|�C���^
	//pActor:�A�N�^�̃|�C���^
	//Trans:�������W
	StageObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidStatic> pActor, Transform Trans);

	//�f�X�g���N�^
	~StageObject();

	//������
	void Init(XMVECTOR Pos);
	//�X�V
	void Update();
	//�R�s�[
	void CopyObject(StageObject** Object);
	//���
	void Release()override;
};

