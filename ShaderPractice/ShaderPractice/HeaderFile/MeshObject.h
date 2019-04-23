#pragma once

#include "GameObject.h"
#include "Mesh.h"
#include "MyAnimation.h"
#include "D3Device.h"
#include "Camera.h"
#include "Light.h"

//���b�V����\������N���X
class MeshObject :public GameObject
{
protected:
	shared_ptr<vector<MyMeshData>>		m_pMesh;			//���b�V��
	
public:

	bool					IsAppear;	//�\�����Ă�����
	ComPtr<ID3D11Buffer>	m_pMatrixBuffer;//�p���o�b�t�@

	Transform				m_WorldMat;	//���[���h���W
	
	//�R���X�g���N�^
	MeshObject();
	//�R���X�g���N�^
	//pMesh:���b�V���̃|�C���^
	MeshObject(vector<MyMeshData> pMesh);
	//�R���X�g���N�^
	//pMesh:���b�V���̃|�C���^
	//Trans:�������W
	MeshObject(vector<MyMeshData> pMesh,Transform Trans);
	//�R���X�g���N�^
	//FileName:�t�@�C����
	//pDevice:�f�o�C�X�̃|�C���^
	MeshObject(const char* FileName, ComPtr<ID3D11Device> pDevice);
	//�R���X�g���N�^
	//FileName:�t�@�C����
	//pDevice:�f�o�C�X�̃|�C���^
	//Trans:�������W
	MeshObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans);
	//�f�X�g���N�^
	~MeshObject();

	//���b�V���쐬
	virtual void	CreateMesh(const char* ModelPath, ComPtr<ID3D11Device> pDevice);						
	//�X�V
	void			Update();																			
	//�`��
	virtual void	Draw(Camera Cam,vector<Light> ProjctLight,ComPtr<ID3D11DeviceContext> pRenderer);	
	//�R�s�[
	void			CopyObject(shared_ptr<MeshObject>& ppObject);										
	//���
	void			Release();																			
};

