#pragma once
#include"Transform.h"
#include"MySkelton.h"

using namespace fbxsdk;
using namespace Microsoft::WRL;
using namespace std;

//シェーダに渡す値
struct ShaderGrobal
{
	XMMATRIX WorldMat;//ワールド座標行列
	XMMATRIX ViewMat; //ビュー行列
	XMMATRIX ProjMat; //プロジェクション行列
	XMFLOAT4 LightVec;//ライトベクトル
};

//ランバートマテリアル
struct MyLambert
{
	XMFLOAT4 Ambient;//環境光
	XMFLOAT3 Diffuse;//拡散反射光
	float	 Alpha;  //透過
};

//フォングマテリアル
struct MyPhong : public MyLambert
{
	XMFLOAT3 Specular;
	float Shiness;
	float Refrect;//反射光
};

//テクスチャ群
class TextureStocker
{
public:
	//ファイル名でインデックスされたテクスチャ
	map<string, ID3D11ShaderResourceView*> Textures;
};

//頂点
class MyVertex
{
public:
	//頂点座標
	XMFLOAT4 Position;
	//法線ベクトル
	XMFLOAT3 Normal;
	//UV座標
	XMFLOAT2 UV;
};

//シェーダへ渡す頂点
class VertexStruct
{
public:
	//位置
	XMFLOAT4 Position;
	//法線ベクトル
	XMFLOAT3 Normal;
	//UV座標
	XMFLOAT2 UV;
	//対応するボーン
	XMFLOAT4 BoneId;
	//ボーンの重み
	XMFLOAT4 BoneWeight;
};

//シェーダへ渡すボーンの姿勢
class BoneMatData
{
public:
	XMMATRIX BoneMat[4];
};

//ポリゴン
class MyPolygon
{
public:
	vector<int> PolygonIndex;
};

//メッシュ
class MyMesh
{
public:
	//コンストラクタ
	MyMesh();
	//コンストラクタ
	//Vertex       : 頂点座標
	//PolygonIndex : ポリゴンを作る頂点のインデックス
	MyMesh(vector<MyVertex> Vertex, vector<int> PolygonIndex);
	//Positions:頂点座標
	//Normals:法線ベクトル
	//UVs:UV座標
	//Index:ポリゴンインデックス
	MyMesh(vector<XMFLOAT4> Positions, vector<XMFLOAT3> Normals, vector<XMFLOAT2> UVs, vector<int> Index);
	//デストラクタ
	~MyMesh();

	//頂点数
	int VertexCount;	
	//ポリゴン数
	int PolygonCount;	
	//インデックスの総頂点数
	int PolygonVertexCount;

	//頂点情報
	shared_ptr<vector<MyVertex>>	m_pVertex;	
	//ボーンによる頂点への影響
	shared_ptr<vector<RefBone>>		m_pRefBone; 

	vector<int>			PolygonIndex;//ポリゴンインデックス
	int MaterialId;
};

//メッシュオブジェクト マテリアルと1：1の関係で1モデルで複数作られる
class MyMeshData
{
protected:
	//メッシュのポインタ
	shared_ptr<MyMesh>							m_pMesh;	
	//テクスチャ
	vector<ComPtr<ID3D11ShaderResourceView>>	m_pTexture;
	
public:
	//マテリアル
	MyLambert									m_Material; 

	//シェーダ
	ComPtr<ID3D11VertexShader>		m_pVShader;
	ComPtr<ID3D11PixelShader>		m_pPShader;

	//各バッファ
	ComPtr<ID3D11Buffer>			m_pVertexBuffer;
	ComPtr<ID3D11Buffer>			m_pIndexBuffer;
	ComPtr<ID3D11Buffer>			m_pMaterialBuffer;
	ComPtr<ID3D11SamplerState>		m_pSampleLiner;

	//コンストラクタ
	MyMeshData();
	//コンストラクタ
	//Mesh:メッシュ
	//Material:マテリアル
	MyMeshData(MyMesh Mesh,MyLambert Material, vector<ComPtr<ID3D11ShaderResourceView>> Tex);
	//デストラクタ
	~MyMeshData();

	//頂点数
	int					GetVertCount();			
	//インデックス数
	int					GetIndexCount();		
	//ポリゴンを作っている全頂点
	int					GetPolyVertexCount();	
	//テクスチャ数
	int					GetTexCount();			
	//メッシュのポインタ
	shared_ptr<MyMesh>	GetMesh();				
	//頂点位置
	XMFLOAT4			GetVertPos(int Index);	
	//頂点インデックス
	vector<int>			GetIndex();				
	//テクスチャ
	ComPtr<ID3D11ShaderResourceView> GetTexture(int Index);

	//メッシュ
	void SetMesh(shared_ptr<MyMesh> Mesh);			
	//マテリアル
	void SetMaterial(MyLambert Material);		
	// テクスチャ
	void SetTexture(vector<ComPtr<ID3D11ShaderResourceView>> SRView);
	//テクスチャ追加
	void AddTexture(ComPtr<ID3D11ShaderResourceView> SRView);

	//各バッファの鋳型を作る関数
	//頂点
	void CreateVertBuffer(ComPtr<ID3D11Device> pDevice);	
	//インデックス
	void CreateIndexBuffer(ComPtr<ID3D11Device> pDevice);
	//マテリアル
	void CreateMaterialBuffer(ComPtr<ID3D11Device> pDevice);

	//頂点シェーダ
	void SetVShader(ComPtr<ID3D11VertexShader> pVShader);
	//ピクセルシェーダ
	void SetPShader(ComPtr<ID3D11PixelShader> pPShader);
	//解放
	void Release();
};