#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "D3Device.h"

class SpriteObject :public GameObject
{
	//スプライトのポインタ
	shared_ptr<Sprite>	m_pSprite;
public:
	//表示するか
	bool				IsAppear;
	//座標
	Transform			m_WorldMat;

	//シェーダへ渡すバッファ
	ComPtr<ID3D11Buffer>			m_pMatrixBuffer;

	//コンストラクタ
	SpriteObject();
	//デストラクタ
	~SpriteObject();

	//メッシュの設定
	void SetMeshObject(shared_ptr<Sprite>& pSprite);
	//メッシュの取得
	Sprite* GetMeshObject();

	//スプライトの作成
	void Create(const char* TexPath, ComPtr<ID3D11Device> pDevice);
	//更新
	void Update();
	//描画
	void Draw(Camera Cam,ComPtr<ID3D11DeviceContext> pDevContext, ID3D11VertexShader* VShader, ID3D11PixelShader* PShader);
	//オブジェクトのコピー
	void CopyObject(SpriteObject** ppObject);
	//解放
	void Release();
};

