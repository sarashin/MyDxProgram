#pragma once
#include"Mesh.h"

class Sprite
{
protected:
	//板ポリ
	shared_ptr<MyMesh> m_pMesh;
	//テクスチャリソース
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
public:
	//マテリアル
	MyLambert m_Material;
	//コンストラクタ
	Sprite();
	//コンストラクタ
	Sprite(const Sprite& sprite);
	//コンストラクタ
	//Mesh:メッシュ
	//pTexture:テクスチャ
	Sprite(MyMesh Mesh,ID3D11ShaderResourceView* pTexture);
	//デストラクタ
	~Sprite();

	//シェーダへ渡すバッファ
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	ComPtr<ID3D11Buffer>		m_pIndexBuffer;
	ComPtr<ID3D11Buffer>		m_pMaterialBuffer;
	ComPtr<ID3D11SamplerState>  m_pSampleLiner;

	//シェーダ
	shared_ptr<ID3D11VertexShader> m_pVShader;
	shared_ptr<ID3D11PixelShader>  m_pPShader;

	//座標
	Transform m_LocalMat;

	//頂点数取得
	int							GetVertCount();
	//インデックス数取得
	int							GetIndexCount(int i);
	//メッシュ取得
	MyMesh						GetMesh();
	//マテリアル取得
	MyLambert					GetMaterial();

	//テクスチャ取得
	ID3D11ShaderResourceView**	GetTexture();

	//メッシュの設定
	void SetMesh(MyMesh Mesh);
	//テクスチャ設定
	void SetTexture(ID3D11ShaderResourceView* SRView);
	//マテリアル設定
	void SetMaterial(MyLambert Material);
	//バッファ作成
	void CreateVertBuffer(ID3D11Device* Device);
	void CreateIndexBuffer(ID3D11Device* Device);
	void CreateMaterialBuffer(ID3D11Device* Device);

	//解放
	void Release();
};

