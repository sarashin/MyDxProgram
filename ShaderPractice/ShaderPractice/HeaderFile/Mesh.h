#pragma once
#include"Transform.h"
#include"MySkelton.h"

using namespace fbxsdk;
using namespace Microsoft::WRL;
using namespace std;

//�V�F�[�_�ɓn���l
struct ShaderGrobal
{
	XMMATRIX WorldMat;//���[���h���W�s��
	XMMATRIX ViewMat; //�r���[�s��
	XMMATRIX ProjMat; //�v���W�F�N�V�����s��
	XMFLOAT4 LightVec;//���C�g�x�N�g��
};

//�����o�[�g�}�e���A��
struct MyLambert
{
	XMFLOAT4 Ambient;//����
	XMFLOAT3 Diffuse;//�g�U���ˌ�
	float	 Alpha;  //����
};

//�t�H���O�}�e���A��
struct MyPhong : public MyLambert
{
	XMFLOAT3 Specular;
	float Shiness;
	float Refrect;//���ˌ�
};

//�e�N�X�`���Q
class TextureStocker
{
public:
	//�t�@�C�����ŃC���f�b�N�X���ꂽ�e�N�X�`��
	map<string, ID3D11ShaderResourceView*> Textures;
};

//���_
class MyVertex
{
public:
	//���_���W
	XMFLOAT4 Position;
	//�@���x�N�g��
	XMFLOAT3 Normal;
	//UV���W
	XMFLOAT2 UV;
};

//�V�F�[�_�֓n�����_
class VertexStruct
{
public:
	//�ʒu
	XMFLOAT4 Position;
	//�@���x�N�g��
	XMFLOAT3 Normal;
	//UV���W
	XMFLOAT2 UV;
	//�Ή�����{�[��
	XMFLOAT4 BoneId;
	//�{�[���̏d��
	XMFLOAT4 BoneWeight;
};

//�V�F�[�_�֓n���{�[���̎p��
class BoneMatData
{
public:
	XMMATRIX BoneMat[4];
};

//�|���S��
class MyPolygon
{
public:
	vector<int> PolygonIndex;
};

//���b�V��
class MyMesh
{
public:
	//�R���X�g���N�^
	MyMesh();
	//�R���X�g���N�^
	//Vertex       : ���_���W
	//PolygonIndex : �|���S������钸�_�̃C���f�b�N�X
	MyMesh(vector<MyVertex> Vertex, vector<int> PolygonIndex);
	//Positions:���_���W
	//Normals:�@���x�N�g��
	//UVs:UV���W
	//Index:�|���S���C���f�b�N�X
	MyMesh(vector<XMFLOAT4> Positions, vector<XMFLOAT3> Normals, vector<XMFLOAT2> UVs, vector<int> Index);
	//�f�X�g���N�^
	~MyMesh();

	//���_��
	int VertexCount;	
	//�|���S����
	int PolygonCount;	
	//�C���f�b�N�X�̑����_��
	int PolygonVertexCount;

	//���_���
	shared_ptr<vector<MyVertex>>	m_pVertex;	
	//�{�[���ɂ�钸�_�ւ̉e��
	shared_ptr<vector<RefBone>>		m_pRefBone; 

	vector<int>			PolygonIndex;//�|���S���C���f�b�N�X
	int MaterialId;
};

//���b�V���I�u�W�F�N�g �}�e���A����1�F1�̊֌W��1���f���ŕ��������
class MyMeshData
{
protected:
	//���b�V���̃|�C���^
	shared_ptr<MyMesh>							m_pMesh;	
	//�e�N�X�`��
	vector<ComPtr<ID3D11ShaderResourceView>>	m_pTexture;
	
public:
	//�}�e���A��
	MyLambert									m_Material; 

	//�V�F�[�_
	ComPtr<ID3D11VertexShader>		m_pVShader;
	ComPtr<ID3D11PixelShader>		m_pPShader;

	//�e�o�b�t�@
	ComPtr<ID3D11Buffer>			m_pVertexBuffer;
	ComPtr<ID3D11Buffer>			m_pIndexBuffer;
	ComPtr<ID3D11Buffer>			m_pMaterialBuffer;
	ComPtr<ID3D11SamplerState>		m_pSampleLiner;

	//�R���X�g���N�^
	MyMeshData();
	//�R���X�g���N�^
	//Mesh:���b�V��
	//Material:�}�e���A��
	MyMeshData(MyMesh Mesh,MyLambert Material, vector<ComPtr<ID3D11ShaderResourceView>> Tex);
	//�f�X�g���N�^
	~MyMeshData();

	//���_��
	int					GetVertCount();			
	//�C���f�b�N�X��
	int					GetIndexCount();		
	//�|���S��������Ă���S���_
	int					GetPolyVertexCount();	
	//�e�N�X�`����
	int					GetTexCount();			
	//���b�V���̃|�C���^
	shared_ptr<MyMesh>	GetMesh();				
	//���_�ʒu
	XMFLOAT4			GetVertPos(int Index);	
	//���_�C���f�b�N�X
	vector<int>			GetIndex();				
	//�e�N�X�`��
	ComPtr<ID3D11ShaderResourceView> GetTexture(int Index);

	//���b�V��
	void SetMesh(shared_ptr<MyMesh> Mesh);			
	//�}�e���A��
	void SetMaterial(MyLambert Material);		
	// �e�N�X�`��
	void SetTexture(vector<ComPtr<ID3D11ShaderResourceView>> SRView);
	//�e�N�X�`���ǉ�
	void AddTexture(ComPtr<ID3D11ShaderResourceView> SRView);

	//�e�o�b�t�@�̒��^�����֐�
	//���_
	void CreateVertBuffer(ComPtr<ID3D11Device> pDevice);	
	//�C���f�b�N�X
	void CreateIndexBuffer(ComPtr<ID3D11Device> pDevice);
	//�}�e���A��
	void CreateMaterialBuffer(ComPtr<ID3D11Device> pDevice);

	//���_�V�F�[�_
	void SetVShader(ComPtr<ID3D11VertexShader> pVShader);
	//�s�N�Z���V�F�[�_
	void SetPShader(ComPtr<ID3D11PixelShader> pPShader);
	//���
	void Release();
};