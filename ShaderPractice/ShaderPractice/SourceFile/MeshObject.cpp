#include "stdafx.h"
#include "MeshObject.h"
#include"D3Device.h"
#include"MeshObjectFactory.h"

//�R���X�g���N�^
MeshObject::MeshObject() : GameObject()
{
	IsAppear = true;
	m_WorldMat = Transform();
}

MeshObject::MeshObject(vector<MyMeshData> Mesh) : GameObject()
{
	m_pMesh=make_shared<vector<MyMeshData>>(Mesh);
	IsAppear = true;
}

MeshObject::MeshObject(vector<MyMeshData> Mesh,Transform Trans) : GameObject(Trans)
{
	m_pMesh = make_shared<vector<MyMeshData>>(Mesh);
	IsAppear = true;
}

MeshObject::MeshObject(const char* FileName, ComPtr<ID3D11Device> pDevice) : GameObject()
{
	CreateMesh(FileName,pDevice);
	IsAppear = true;
}

MeshObject::MeshObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans) : GameObject(Trans)
{
	CreateMesh(FileName, pDevice);
	IsAppear = true;
}

//�f�X�g���N�^
MeshObject::~MeshObject()
{
	Release();
}

//���b�V���쐬
void MeshObject::CreateMesh(const char* Path,ComPtr<ID3D11Device> pDevice)
{
	m_pMesh = make_shared<vector<MyMeshData>>();

	//���b�V���쐬
	if (MeshFactory::GetInstance().CreateMeshObject(Path, m_pMesh, NULL, NULL)==false)
	{
		m_pMesh = NULL;
	}

	//�p���̃o�b�t�@�쐬
	D3D11_BUFFER_DESC MatBDesc;
	MatBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatBDesc.ByteWidth = sizeof(ShaderGrobal);
	MatBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatBDesc.MiscFlags = 0;
	MatBDesc.StructureByteStride = 0;
	MatBDesc.Usage = D3D11_USAGE_DYNAMIC;

	pDevice->CreateBuffer(&MatBDesc, NULL, &m_pMatrixBuffer);
}

//�`��
void MeshObject::Draw(Camera ProjCam, vector<Light> ProjLight,ComPtr<ID3D11DeviceContext> pDevContext)
{
	if (pDevContext == NULL)
		return;
	if (m_pMesh == NULL)
		return;
	if (IsAppear == false)
		return;

	//�V�F�[�_�֓n���p���f�[�^
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDevContext->Map(m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ShaderGrobal SG;
		SG.ProjMat = XMMatrixTranspose(ProjCam.GetProjMat());
		SG.ViewMat = XMMatrixTranspose(ProjCam.GetViewMatW());
		SG.WorldMat = XMMatrixTranspose(m_WorldMat.GetMat());
		SG.LightVec = ProjLight[0].m_LightVec;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&SG, sizeof(ShaderGrobal));
		pDevContext->Unmap(m_pMatrixBuffer.Get(), 0);
	}

	pDevContext->VSSetConstantBuffers(0, 1, m_pMatrixBuffer.GetAddressOf());//�V�F�[�_�ɃZ�b�g
	
	int Size = m_pMesh->size();
	UINT Stride = sizeof(VertexStruct);	//���_�\���̃T�C�Y
	UINT Offset = 0;					//
	MyMeshData ThisMesh;				//�`�悷�郁�b�V��

	//���b�V�����Ƃɕ`��
	for (int i = 0; i < Size; i++)
	{
		ThisMesh = (*m_pMesh)[i];

		//�V�F�[�_���Z�b�g
		pDevContext->VSSetShader(ThisMesh.m_pVShader.Get(), NULL, 0);	//���_
		pDevContext->PSSetShader(ThisMesh.m_pPShader.Get(), NULL, 0);	//�s�N�Z��

		//���b�V�������V�F�[�_�֓n��
		pDevContext->IASetVertexBuffers(0, 1, ThisMesh.m_pVertexBuffer.GetAddressOf(), &Stride, &Offset);	//���_���
		pDevContext->IASetIndexBuffer(ThisMesh.m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);				//���_�C���f�b�N�X

		//�}�e���A�������V�F�[�_�֓n��
		D3D11_MAPPED_SUBRESOURCE MaterialData;
		if (SUCCEEDED(pDevContext->Map(ThisMesh.m_pMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MaterialData)))
		{
			MyLambert Lambert;
			Lambert = ThisMesh.m_Material;
			memcpy_s(MaterialData.pData, MaterialData.RowPitch, &Lambert, sizeof(MyLambert));
			pDevContext->Unmap(ThisMesh.m_pMaterialBuffer.Get(), 0);
		}

		pDevContext->PSSetConstantBuffers(0, 1, ThisMesh.m_pMaterialBuffer.GetAddressOf());	//�}�e���A��
		pDevContext->PSSetSamplers(0, 1, ThisMesh.m_pSampleLiner.GetAddressOf());			//�e�N�X�`���T���v��
		pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);			//�g�|���W

		int TexSize = ThisMesh.GetTexCount();

		for (int j = 0; j < TexSize; j++)
		{
			if (ThisMesh.GetTexture(j) != NULL)
			{
				pDevContext->PSSetShaderResources(0, 1, ThisMesh.GetTexture(j).GetAddressOf());//�e�N�X�`��
			}
		}

		pDevContext->DrawIndexedInstanced(ThisMesh.GetPolyVertexCount(), 1, 0, 0, 0);	//�`��
	}
}

//�X�V����
void MeshObject::Update()
{

}

//�I�u�W�F�N�g�̃R�s�[
void MeshObject::CopyObject(shared_ptr<MeshObject>& ppObjects)
{
	
}

//���
void MeshObject::Release()
{
	m_pMatrixBuffer.Reset();
	m_pMesh.reset();
}