#include "stdafx.h"
#include "MeshObject.h"
#include"D3Device.h"
#include"MeshObjectFactory.h"

//コンストラクタ
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

//デストラクタ
MeshObject::~MeshObject()
{
	Release();
}

//メッシュ作成
void MeshObject::CreateMesh(const char* Path,ComPtr<ID3D11Device> pDevice)
{
	m_pMesh = make_shared<vector<MyMeshData>>();

	//メッシュ作成
	if (MeshFactory::GetInstance().CreateMeshObject(Path, m_pMesh, NULL, NULL)==false)
	{
		m_pMesh = NULL;
	}

	//姿勢のバッファ作成
	D3D11_BUFFER_DESC MatBDesc;
	MatBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatBDesc.ByteWidth = sizeof(ShaderGrobal);
	MatBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatBDesc.MiscFlags = 0;
	MatBDesc.StructureByteStride = 0;
	MatBDesc.Usage = D3D11_USAGE_DYNAMIC;

	pDevice->CreateBuffer(&MatBDesc, NULL, &m_pMatrixBuffer);
}

//描画
void MeshObject::Draw(Camera ProjCam, vector<Light> ProjLight,ComPtr<ID3D11DeviceContext> pDevContext)
{
	if (pDevContext == NULL)
		return;
	if (m_pMesh == NULL)
		return;
	if (IsAppear == false)
		return;

	//シェーダへ渡す姿勢データ
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

	pDevContext->VSSetConstantBuffers(0, 1, m_pMatrixBuffer.GetAddressOf());//シェーダにセット
	
	int Size = m_pMesh->size();
	UINT Stride = sizeof(VertexStruct);	//頂点構造体サイズ
	UINT Offset = 0;					//
	MyMeshData ThisMesh;				//描画するメッシュ

	//メッシュごとに描画
	for (int i = 0; i < Size; i++)
	{
		ThisMesh = (*m_pMesh)[i];

		//シェーダをセット
		pDevContext->VSSetShader(ThisMesh.m_pVShader.Get(), NULL, 0);	//頂点
		pDevContext->PSSetShader(ThisMesh.m_pPShader.Get(), NULL, 0);	//ピクセル

		//メッシュ情報をシェーダへ渡す
		pDevContext->IASetVertexBuffers(0, 1, ThisMesh.m_pVertexBuffer.GetAddressOf(), &Stride, &Offset);	//頂点情報
		pDevContext->IASetIndexBuffer(ThisMesh.m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);				//頂点インデックス

		//マテリアル情報をシェーダへ渡す
		D3D11_MAPPED_SUBRESOURCE MaterialData;
		if (SUCCEEDED(pDevContext->Map(ThisMesh.m_pMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MaterialData)))
		{
			MyLambert Lambert;
			Lambert = ThisMesh.m_Material;
			memcpy_s(MaterialData.pData, MaterialData.RowPitch, &Lambert, sizeof(MyLambert));
			pDevContext->Unmap(ThisMesh.m_pMaterialBuffer.Get(), 0);
		}

		pDevContext->PSSetConstantBuffers(0, 1, ThisMesh.m_pMaterialBuffer.GetAddressOf());	//マテリアル
		pDevContext->PSSetSamplers(0, 1, ThisMesh.m_pSampleLiner.GetAddressOf());			//テクスチャサンプラ
		pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);			//トポロジ

		int TexSize = ThisMesh.GetTexCount();

		for (int j = 0; j < TexSize; j++)
		{
			if (ThisMesh.GetTexture(j) != NULL)
			{
				pDevContext->PSSetShaderResources(0, 1, ThisMesh.GetTexture(j).GetAddressOf());//テクスチャ
			}
		}

		pDevContext->DrawIndexedInstanced(ThisMesh.GetPolyVertexCount(), 1, 0, 0, 0);	//描画
	}
}

//更新処理
void MeshObject::Update()
{

}

//オブジェクトのコピー
void MeshObject::CopyObject(shared_ptr<MeshObject>& ppObjects)
{
	
}

//解放
void MeshObject::Release()
{
	m_pMatrixBuffer.Reset();
	m_pMesh.reset();
}