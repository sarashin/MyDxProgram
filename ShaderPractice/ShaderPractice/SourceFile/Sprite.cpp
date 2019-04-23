#include"stdafx.h"
#include "Sprite.h"



Sprite::Sprite()
{
	
}

Sprite::Sprite(const Sprite& sprite)
{
	this->m_pMesh = sprite.m_pMesh;
	this->m_pTexture.Attach(sprite.m_pTexture.Get());
	this->m_Material = sprite.m_Material;
}

Sprite::Sprite(MyMesh Mesh, ID3D11ShaderResourceView* pTexture)
{
	m_pMesh = make_shared<MyMesh>(Mesh);
	m_pTexture.Attach(pTexture);
}

Sprite::~Sprite()
{
}

ID3D11ShaderResourceView** Sprite::GetTexture()
{
	if(m_pTexture!=NULL)
		return m_pTexture.GetAddressOf();

		return NULL;
}

int Sprite::GetVertCount()
{
	return m_pMesh->VertexCount;
}

int Sprite::GetIndexCount(int IndexNum)
{
	return m_pMesh->PolygonIndex.size();
}

MyMesh Sprite::GetMesh()
{
	return *m_pMesh;
}

MyLambert Sprite::GetMaterial()
{
	return m_Material;
}

//メッシュのセット
void Sprite::SetMesh(MyMesh pMesh)
{
	*m_pMesh = pMesh;
}

//マテリアルのセット
void Sprite::SetMaterial(MyLambert Material)
{
	m_Material=Material;
}

//テクスチャのセット
void Sprite::SetTexture(ID3D11ShaderResourceView* SRView)
{
	if (SRView == NULL)
	{
		return;
	}

	m_pTexture.Attach(SRView);
}

//インデックスバッファ作成
void Sprite::CreateIndexBuffer(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int)*GetIndexCount(0);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitIBData;
	InitIBData.pSysMem = m_pMesh->PolygonIndex.data();
	InitIBData.SysMemPitch = 0;
	InitIBData.SysMemSlicePitch = 0;
	pDevice->CreateBuffer(&bd, &InitIBData, &m_pIndexBuffer);
}

//頂点バッファ作成
void Sprite::CreateVertBuffer(ID3D11Device* pDevice)
{
	MyVertex Vert;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MyVertex) * m_pMesh->VertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitVBData;
	InitVBData.pSysMem = m_pMesh->m_pVertex.get()->data();
	InitVBData.SysMemPitch = 0;
	InitVBData.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&bd, &InitVBData, m_pVertexBuffer.GetAddressOf());
}

//マテリアルバッファ作成
void Sprite::CreateMaterialBuffer(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC MBDesc;
	MBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MBDesc.ByteWidth = sizeof(MyLambert);
	MBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MBDesc.MiscFlags = 0;
	MBDesc.StructureByteStride = 0;
	MBDesc.Usage = D3D11_USAGE_DYNAMIC;

	pDevice->CreateBuffer(&MBDesc, NULL, m_pMaterialBuffer.GetAddressOf());
}

//解放
void Sprite::Release()
{
	m_pIndexBuffer.Reset();
	m_pMaterialBuffer.Reset();
	m_pVertexBuffer.Reset();
	m_pSampleLiner.Reset();
	m_pTexture.Reset();
}