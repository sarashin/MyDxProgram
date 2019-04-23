#pragma once
#include"Mesh.h"
#include"MySkelton.h"

using namespace fbxsdk;
using namespace Microsoft::WRL;
using namespace std;


//�A�j���[�V����
class MySkinAnimation
{
public:
	//�R���X�g���N�^
	MySkinAnimation();
	//�f�X�g���N�^
	~MySkinAnimation();

	//�e�t���[���̎p��(��{�[��)
	vector<XMMATRIX>		 m_AnimMatrix;	  
	//�e�t���[���̎p��(���̑��{�[��)
	vector<vector<XMMATRIX>> m_AnimBoneMatrix;

	//�J�n�t���[��
	int Start;						
	//�I���t���[��
	int Stop;						

	//���O
	string Name;					

	int GetBoneNum();
	void Release();
};

//�A�j���[�V�����R���e�i
class MyAnimationContainer
{
	//�A�j���[�V�����R���e�i
	shared_ptr<vector<MySkinAnimation>> m_pAnimation;	
	//�C���f�b�N�X
	map<string,int>			m_AnimIndex;				
	//���b�V���ւ̃|�C���^
	shared_ptr<MyMeshData>  m_pMesh;					
	//���A�j���[�V�����̃C���f�b�N�X�i���o�[
	int						NowAnimNum;					
	//���O
	string					NowAnimName;				
public:
	//�R���X�g���N�^
	MyAnimationContainer();
	//�R���X�g���N�^
	//Animation:�A�j���[�V�������
	MyAnimationContainer(vector<MySkinAnimation> Animation);

	//�A�j���[�V�����X�V
	void UpdateAnimation(int AniIndex,float Frame, MySkelton& pSkelton);
};