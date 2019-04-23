#include"stdafx.h"
#include"Mesh.h"
#include"D3Device.h"

//コンストラクタ
MyMesh::MyMesh()
{
	m_pVertex = make_shared<vector<MyVertex>>();
}

MyMesh::MyMesh(vector<MyVertex> pVertex,vector<int> Index)
{
	m_pVertex = make_shared<vector<MyVertex>>(pVertex);
	PolygonIndex = Index;
	VertexCount = m_pVertex->size();
	PolygonCount = Index.size();
	PolygonVertexCount = Index.size() * 3;
}

MyMesh::MyMesh(vector<XMFLOAT4> Positions, vector<XMFLOAT3> Normals, vector<XMFLOAT2> UVs, vector<int> Index)
{
	
	PolygonIndex = Index;
	VertexCount = Positions.size();
	PolygonCount = Index.size() / 3;
	PolygonVertexCount = Index.size();
}

//デストラクタ
MyMesh::~MyMesh()
{}

//コンストラクタ
MyMeshData::MyMeshData()
{
	
}

MyMeshData::MyMeshData(MyMesh pMesh, MyLambert Lambert, vector<ComPtr<ID3D11ShaderResourceView>> pTex)
{
	m_pMesh = make_shared<MyMesh>();

	*m_pMesh = pMesh;
	m_Material = Lambert;
	m_pTexture = pTex;
	
}

//デストラクタ
MyMeshData::~MyMeshData()
{
	Release();
}

//頂点数取得
int MyMeshData::GetVertCount()
{
	return m_pMesh->m_pVertex->size();
}

//頂点インデックス数取得
int MyMeshData::GetIndexCount()
{
	return m_pMesh->PolygonIndex.size();
}

//全ポリゴンの頂点数
int MyMeshData::GetPolyVertexCount()
{
	return m_pMesh->PolygonIndex.size();
}

//テクスチャ数
int MyMeshData::GetTexCount()
{
	return m_pTexture.size();
}

//メッシュ取得
shared_ptr<MyMesh> MyMeshData::GetMesh()
{
	return m_pMesh;
}

//テクスチャ取得
ComPtr<ID3D11ShaderResourceView> MyMeshData::GetTexture(int Index)
{
	if (Index < m_pTexture.size())
		return m_pTexture[Index];
	else
		return NULL;
}

//頂点取得
XMFLOAT4 MyMeshData::GetVertPos(int Index)
{
	return m_pMesh->m_pVertex->at(Index).Position;
}

//頂点インデックス取得
vector<int> MyMeshData::GetIndex()
{
	return m_pMesh->PolygonIndex;
}

//メッシュのセット
void MyMeshData::SetMesh(shared_ptr<MyMesh> pMesh)
{
	m_pMesh=pMesh;
}

//マテリアルのセット
void MyMeshData::SetMaterial(MyLambert Material)
{
	m_Material=Material;
}

//テクスチャのセット
void MyMeshData::SetTexture(vector<ComPtr<ID3D11ShaderResourceView>> SRView)
{
	m_pTexture=SRView;
}

//テクスチャ取得
void MyMeshData::AddTexture(ComPtr<ID3D11ShaderResourceView> pSRView)
{
	if (pSRView)
		return;

	m_pTexture.push_back(pSRView);
}

//インデックスバッファ作成
void MyMeshData::CreateIndexBuffer(ComPtr<ID3D11Device> pDevice)
{
	if (pDevice == NULL)
	{
		return;
	}

	m_pIndexBuffer.Reset();
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int)*GetPolyVertexCount();
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
void MyMeshData::CreateVertBuffer(ComPtr<ID3D11Device> pDevice)
{
	if (pDevice == NULL)
	{
		return;
	}

	m_pVertexBuffer.Reset();
	MyVertex Vertex;
	vector<VertexStruct> Vertices;
	RefBone	 Bone;
	int VertexCount;
	VertexCount = m_pMesh->VertexCount;
	Vertices.resize(VertexCount);

	//頂点バッファの形にまとめる
	for (int i = 0; i < VertexCount; i++)
	{
		Vertex = m_pMesh->m_pVertex->at(i);
		Vertices[i].Position = Vertex.Position;
		Vertices[i].Normal = Vertex.Normal;
		Vertices[i].UV = Vertex.UV;

		Bone = m_pMesh->m_pRefBone->at(i);

		switch (Bone.BoneId.size())
		{
		case 4:
			Vertices[i].BoneId.w = Bone.BoneId[3];
			Vertices[i].BoneWeight.w = Bone.Weight[3];
		case 3:
			Vertices[i].BoneId.z = Bone.BoneId[2];
			Vertices[i].BoneWeight.z = Bone.Weight[2];
		case 2:
			Vertices[i].BoneId.y = Bone.BoneId[1];
			Vertices[i].BoneWeight.y = Bone.Weight[1];
		case 1:
			Vertices[i].BoneId.x = Bone.BoneId[0];
			Vertices[i].BoneWeight.x = Bone.Weight[0];
		default:
			break;
		}
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexStruct)*Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitVBData;
	InitVBData.pSysMem = Vertices.data();
	InitVBData.SysMemPitch = 0;
	InitVBData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr=pDevice->CreateBuffer(&bd, &InitVBData, &m_pVertexBuffer);
}

//マテリアルバッファ作成
void MyMeshData::CreateMaterialBuffer(ComPtr<ID3D11Device> pDevice)
{
	if (pDevice == NULL)
	{
		return;
	}

	D3D11_BUFFER_DESC MBDesc;
	MBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MBDesc.ByteWidth = sizeof(MyLambert);
	MBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MBDesc.MiscFlags = 0;
	MBDesc.StructureByteStride = 0;
	MBDesc.Usage = D3D11_USAGE_DYNAMIC;

	pDevice->CreateBuffer(&MBDesc, NULL, &m_pMaterialBuffer);

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	pDevice->CreateSamplerState(&SamDesc, m_pSampleLiner.GetAddressOf());
}

void MyMeshData::SetVShader(ComPtr<ID3D11VertexShader> pVShader)
{
	m_pVShader = pVShader;
}

void MyMeshData::SetPShader(ComPtr<ID3D11PixelShader> pPShader)
{
	m_pPShader = pPShader;
}

//解放
void MyMeshData::Release()
{
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();
	m_pMaterialBuffer.Reset();
	m_pMesh.reset();
	m_pPShader.Reset();
	m_pVShader.Reset();

	for (int i = 0; i < m_pTexture.size(); i++)
		m_pTexture[i].Reset();

	m_pSampleLiner.Reset();
}