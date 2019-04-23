#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "D3Device.h"
#include "Mesh.h"
#include "MyAnimation.h"


class MeshFactory :public Singleton<MeshFactory>
{
	//テクスチャを保持
	shared_ptr<TextureStocker>	m_pTexture;
	shared_ptr<D3DRenderer>		m_pDevice;

	FbxManager* m_pFbxManager;
	FbxImporter* m_pImporter;
	FbxIOSettings* m_pIOSettings;

	////FBX
	//頂点情報の作成
	void CreateVertices(FbxMesh* pMesh, vector<MyVertex>& pVertex);
	void SetIndex(FbxMesh* pMesh, vector<int>& pIndex);
	void SetVertex(FbxMesh* pMesh, vector<XMFLOAT4>& pVertex);
	void SetNormal(FbxLayer* Layer, vector<XMFLOAT3>& pNormal);
	void SetUV(FbxLayer* pLayer, vector<XMFLOAT2>& pUV);

	//メッシュ情報の作成
	void CreatePolyGroup(FbxMesh* pMesh, vector<MyMesh>& pMyMesh);

	//マテリアルの作成
	void CreateMaterial(FbxMesh* pMesh, MyLambert& MeshData, vector<ComPtr<ID3D11ShaderResourceView>>& pTex, int MeshNum);

	//アニメーションの作成
	void CreateAnimInfo(FbxMesh* pMesh, vector<RefBone>& pBone, vector<MySkinAnimation>& pMeshData, shared_ptr<MySkelton> pSkelton);
	void CreateBoneInfo(FbxMesh* pMesh, vector<RefBone>& pBone, MySkinAnimation& MeshData, shared_ptr<MySkelton> pSkelton, int StartFrame,int StopFrame);

public:
	MeshFactory();
	~MeshFactory();
	//初期化
	void Init(shared_ptr<D3DRenderer> pDevice);
	//メッシュコンテナ作成
	bool CreateMeshObject(const char* FName,shared_ptr<vector<MyMeshData>> pMeshContainer, shared_ptr<MyAnimationContainer> pAnimContainer, shared_ptr<MySkelton> pSkelton);
	//スプライト作成
	bool CreateSprite(const char* FName, shared_ptr<Sprite>& OptSprite,RECT Rect,RECT Size);
	void Release();
};

