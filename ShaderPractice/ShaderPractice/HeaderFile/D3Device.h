#pragma once
#include"Mesh.h"
#include"Camera.h"
#include"MeshObject.h"
#include"SpriteObject.h"

#define SAFE_RELEASE(x) if(x){x->Release();x=NULL;}

class D3DRenderer
{
private:
	//DirectXの変数
	//デバイス
	ComPtr<ID3D11Device>			m_pDevice;
	//デバイスコンテキスト
	ComPtr<ID3D11DeviceContext>		m_pDevContext;
	//スワップチェイン
	ComPtr<IDXGISwapChain>			m_pSwapChain;
	//レンダーターゲット
	ComPtr<ID3D11RenderTargetView>	m_pRenderTrgView;
	//深度バッファ
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;
	//深度バッファ用テクスチャ
	ComPtr<ID3D11Texture2D>			m_pDepthStencilTex;
	//ブレンドステート
	ComPtr<ID3D11BlendState>		m_pBlendState;
	
protected:
public:
	//デフォルトシェーダ
	ComPtr<ID3D11VertexShader>		m_pDefVShader;
	ComPtr<ID3D11PixelShader>		m_pDefPShader;

	//コンストラクタ
	D3DRenderer();
	//デストラクタ
	~D3DRenderer();

	//ウインドウサイズ
	CRect rect;

	//デバイス取得
	ID3D11Device*			GetDevice();
	//デバイスコンテキスト取得
	ID3D11DeviceContext*	GetDevContext();
	//レンダーターゲット取得
	ID3D11RenderTargetView** GetRenderTarget();

	//シェーダ作成
	//Pass : ファイルパス
	HRESULT CreateVShader(LPCSTR Pass);
	HRESULT CreatePShader(LPCSTR Pass);

	//D3D11作成
	//hWnd:ウインドウハンドル
	HRESULT CreateDevice(HWND hWnd);
	
	//描画開始、終了
	void RenderStart();
	void RenderEnd();
	//深度バッファ無効
	void InvalidDepthStencil();
	//解放
	void Release();
};

//シェーダへ渡す情報
struct ShaderGlobal
{
	XMMATRIX WorldMat;
	XMMATRIX ViewMat;
	XMMATRIX ProjMat;
};