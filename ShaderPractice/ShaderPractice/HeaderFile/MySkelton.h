#pragma once
using namespace Microsoft::WRL;

//�{�[��
class MyBone
{
public:
	//�p��
	XMMATRIX	m_Matrix;	
	//�����p��
	XMMATRIX	m_InitMat;	
	//�e�{�[���̃|�C���^
	vector<shared_ptr<MyBone>> pParent;
	//�q�{�[���̃|�C���^
	vector<shared_ptr<MyBone>> pChild;
	float BoneId;
};

//�V�F�[�_�֓n���{�[��
class RefBone
{
public:
	//�d��
	vector<float> Weight;
	//�{�[��id
	vector<float> BoneId;
};

//�{�[���̏W��
class MySkelton
{
public:
	//�{�[�����
	vector<MyBone> m_Bone;				
	//�V�F�[�_�֓n���{�[���z��
	ComPtr<ID3D11Buffer> m_BoneBuffer;	

	//�R���X�g���N�^
	MySkelton();
	//�R���X�g���N�^
	//Bones:�{�[�����
	MySkelton(vector<MyBone> Bones);
	//�f�X�g���N�^
	~MySkelton();

	//�{�[�����擾
	int					GetBoneNum();
	//�V�F�[�_�֓n���{�[�����
	void CreateBoneBuffer(ComPtr<ID3D11Device> pDevice);
};

//�V�F�[�_�֓n���{�[���z��
struct BoneMatBuf
{
	vector<XMMATRIX> BoneMat;
};