#include "stdafx.h"
#include "MySkelton.h"



MySkelton::MySkelton(vector<MyBone> Bones)
{
	
}


MySkelton::MySkelton()
{
}

MySkelton::~MySkelton()
{
}

//�{�[�����擾
int MySkelton::GetBoneNum()
{
	return m_Bone.size();
}

//�V�F�[�_�֓n���o�b�t�@�쐬
void MySkelton::CreateBoneBuffer(ComPtr<ID3D11Device> pDevice)
{
	D3D11_BUFFER_DESC BBDesc;
	BBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BBDesc.ByteWidth = sizeof(XMMATRIX)*GetBoneNum();
	BBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BBDesc.MiscFlags = 0;
	BBDesc.StructureByteStride = 0;
	BBDesc.Usage = D3D11_USAGE_DYNAMIC;

	pDevice->CreateBuffer(&BBDesc, NULL, &m_BoneBuffer);
}