#pragma once
#include"Mesh.h"

class Sprite
{
protected:
	//�|��
	shared_ptr<MyMesh> m_pMesh;
	//�e�N�X�`�����\�[�X
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
public:
	//�}�e���A��
	MyLambert m_Material;
	//�R���X�g���N�^
	Sprite();
	//�R���X�g���N�^
	Sprite(const Sprite& sprite);
	//�R���X�g���N�^
	//Mesh:���b�V��
	//pTexture:�e�N�X�`��
	Sprite(MyMesh Mesh,ID3D11ShaderResourceView* pTexture);
	//�f�X�g���N�^
	~Sprite();

	//�V�F�[�_�֓n���o�b�t�@
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	ComPtr<ID3D11Buffer>		m_pIndexBuffer;
	ComPtr<ID3D11Buffer>		m_pMaterialBuffer;
	ComPtr<ID3D11SamplerState>  m_pSampleLiner;

	//�V�F�[�_
	shared_ptr<ID3D11VertexShader> m_pVShader;
	shared_ptr<ID3D11PixelShader>  m_pPShader;

	//���W
	Transform m_LocalMat;

	//���_���擾
	int							GetVertCount();
	//�C���f�b�N�X���擾
	int							GetIndexCount(int i);
	//���b�V���擾
	MyMesh						GetMesh();
	//�}�e���A���擾
	MyLambert					GetMaterial();

	//�e�N�X�`���擾
	ID3D11ShaderResourceView**	GetTexture();

	//���b�V���̐ݒ�
	void SetMesh(MyMesh Mesh);
	//�e�N�X�`���ݒ�
	void SetTexture(ID3D11ShaderResourceView* SRView);
	//�}�e���A���ݒ�
	void SetMaterial(MyLambert Material);
	//�o�b�t�@�쐬
	void CreateVertBuffer(ID3D11Device* Device);
	void CreateIndexBuffer(ID3D11Device* Device);
	void CreateMaterialBuffer(ID3D11Device* Device);

	//���
	void Release();
};

