#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "D3Device.h"
#include "Mesh.h"
#include "MyAnimation.h"


class MeshFactory :public Singleton<MeshFactory>
{
	//�e�N�X�`����ێ�
	shared_ptr<TextureStocker>	m_pTexture;
	shared_ptr<D3DRenderer>		m_pDevice;

	FbxManager* m_pFbxManager;
	FbxImporter* m_pImporter;
	FbxIOSettings* m_pIOSettings;

	////FBX
	//���_���̍쐬
	void CreateVertices(FbxMesh* pMesh, vector<MyVertex>& pVertex);
	void SetIndex(FbxMesh* pMesh, vector<int>& pIndex);
	void SetVertex(FbxMesh* pMesh, vector<XMFLOAT4>& pVertex);
	void SetNormal(FbxLayer* Layer, vector<XMFLOAT3>& pNormal);
	void SetUV(FbxLayer* pLayer, vector<XMFLOAT2>& pUV);

	//���b�V�����̍쐬
	void CreatePolyGroup(FbxMesh* pMesh, vector<MyMesh>& pMyMesh);

	//�}�e���A���̍쐬
	void CreateMaterial(FbxMesh* pMesh, MyLambert& MeshData, vector<ComPtr<ID3D11ShaderResourceView>>& pTex, int MeshNum);

	//�A�j���[�V�����̍쐬
	void CreateAnimInfo(FbxMesh* pMesh, vector<RefBone>& pBone, vector<MySkinAnimation>& pMeshData, shared_ptr<MySkelton> pSkelton);
	void CreateBoneInfo(FbxMesh* pMesh, vector<RefBone>& pBone, MySkinAnimation& MeshData, shared_ptr<MySkelton> pSkelton, int StartFrame,int StopFrame);

public:
	MeshFactory();
	~MeshFactory();
	//������
	void Init(shared_ptr<D3DRenderer> pDevice);
	//���b�V���R���e�i�쐬
	bool CreateMeshObject(const char* FName,shared_ptr<vector<MyMeshData>> pMeshContainer, shared_ptr<MyAnimationContainer> pAnimContainer, shared_ptr<MySkelton> pSkelton);
	//�X�v���C�g�쐬
	bool CreateSprite(const char* FName, shared_ptr<Sprite>& OptSprite,RECT Rect,RECT Size);
	void Release();
};

