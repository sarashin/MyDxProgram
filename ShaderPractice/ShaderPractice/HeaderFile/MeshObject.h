#pragma once

#include "GameObject.h"
#include "Mesh.h"
#include "MyAnimation.h"
#include "D3Device.h"
#include "Camera.h"
#include "Light.h"

//メッシュを表示するクラス
class MeshObject :public GameObject
{
protected:
	shared_ptr<vector<MyMeshData>>		m_pMesh;			//メッシュ
	
public:

	bool					IsAppear;	//表示していいか
	ComPtr<ID3D11Buffer>	m_pMatrixBuffer;//姿勢バッファ

	Transform				m_WorldMat;	//ワールド座標
	
	//コンストラクタ
	MeshObject();
	//コンストラクタ
	//pMesh:メッシュのポインタ
	MeshObject(vector<MyMeshData> pMesh);
	//コンストラクタ
	//pMesh:メッシュのポインタ
	//Trans:初期座標
	MeshObject(vector<MyMeshData> pMesh,Transform Trans);
	//コンストラクタ
	//FileName:ファイル名
	//pDevice:デバイスのポインタ
	MeshObject(const char* FileName, ComPtr<ID3D11Device> pDevice);
	//コンストラクタ
	//FileName:ファイル名
	//pDevice:デバイスのポインタ
	//Trans:初期座標
	MeshObject(const char* FileName, ComPtr<ID3D11Device> pDevice, Transform Trans);
	//デストラクタ
	~MeshObject();

	//メッシュ作成
	virtual void	CreateMesh(const char* ModelPath, ComPtr<ID3D11Device> pDevice);						
	//更新
	void			Update();																			
	//描画
	virtual void	Draw(Camera Cam,vector<Light> ProjctLight,ComPtr<ID3D11DeviceContext> pRenderer);	
	//コピー
	void			CopyObject(shared_ptr<MeshObject>& ppObject);										
	//解放
	void			Release();																			
};

