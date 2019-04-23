#pragma once
#include "CharactorObject.h"
#include"SoundEffect.h"

//�v���C���[�̃N���X
class PlayerObject :public CharactorObject
{
protected:

public:
	//�W���C���g
	shared_ptr<Joint> m_pJoint;

	//�R���C�_
	shared_ptr<Collider> m_pCollider;

	//�f���Ă���J����
	Camera* Flont;
	
	//������
	void Init();
	//������

	//Trans:�������W
	void Init(Transform Trans);

	//�R���X�g���N�^
	PlayerObject();

	//�R���X�g���N�^
	//FileName:�t�@�C����
	//pDevice:�f�o�C�X�̃|�C���^
	//Shape:�`��
	//Size:�`��̑傫��
	//ColliderShape:�R���C�_�̌`��̑傫��
	//Weight:RigidBody�̏d��
	PlayerObject(const char* FileName, ComPtr<ID3D11Device> pDevice, int Shape, XMVECTOR Size,XMVECTOR ColliderShape,float Weight);

	//�R���X�g���N�^
	//FileName:�t�@�C����
	//pDevice:�f�o�C�X�̃|�C���^
	//Trans:�������W
	//Shape:�`��
	//Size:�`��̑傫��
	//ColliderShape:�R���C�_�̌`��̑傫��
	//Weight:RigidBody�̏d��
	PlayerObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans, int ObjectShape, XMVECTOR ShapeSize, XMVECTOR ColliderShape,float Weight);

	//�R���X�g���N�^
	//pMesh:���b�V���̃|�C���^
	//pActor:�A�N�^�̃|�C���^
	PlayerObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidBody> pActor);

	//pMesh:���b�V���̃|�C���^
	//pActor:�A�N�^�̃|�C���^
	//Trans:�������W
	PlayerObject(shared_ptr<vector<MyMeshData>> pMesh, shared_ptr<RigidBody> pActor, Transform Trans);

	//�f�X�g���N�^
	~PlayerObject();

	//�X�V
	void Update();

	//�I�u�W�F�N�g�̃R�s�[
	void CopyObject(PlayerObject* ppObject);

	//�R���C�_�̃R�[���o�b�N�֐�
	//Ptr:this�|�C���^
	//pActor:�ڐG�����A�N�^�̃|�C���^
	static void CallBackFanc(void* Ptr,PhysXActor* pActor);

	//�W���C���g�쐬
	void SetJoint(PhysXActor* Actor);

	//���
	void Release();
};

