#pragma once
#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))

class Camera
{
	//ターゲットのワールド座標
	XMFLOAT4X4 m_WorldMat;
	//ローカル座標
	XMFLOAT4X4 m_LocalMat;
	//ビュー行列
	XMFLOAT4X4 m_ViewMat;
	//プロジェクション行列
	XMFLOAT4X4 m_ProjMat;
public:
	
	//コンストラクタ
	Camera();

	//デストラクタ
	~Camera();

	//プロジェクション行列設定
	//Rad    : 画角
	//Aspect : アスペクト比
	//ZNear  : カメラの最短表示距離
	//ZFar   : カメラの最長表示距離
	void SetProjMat(float Rad,float Aspect,float ZNear,float ZFar);

	//ビュー行列設定
	//Eye: 視点座標
	//At : 注視ベクトル
	//Up : アップベクトル
	void SetViewMat(XMFLOAT3 Eye,XMFLOAT3 At,XMFLOAT3 Up);
	//カメラのローカルでの移動
	//Trans : 移動距離
	void TranslateL(XMVECTOR Trans);
	//カメラのローカル座標設定
	//Trans : 位置座標
	void SetTranslateL(XMVECTOR Trans);
	//カメラのローカル回転
	//Quatanion : 回転のクオータニオン
	void RotationL(XMVECTOR Quatanion);
	//カメラのローカル回転
	//Rotate:カメラのロール、ピッチ、ヨー回転
	void RotationL(XMFLOAT3 Rotate);
	//カメラのワールド座標移動
	//Trans:移動距離
	void TranslateW(XMVECTOR Trans);
	//カメラのワールド座標設定
	//Trans:移動座標
	void SetTranslateW(XMVECTOR Trans);
	void RotationW(XMVECTOR Quatanion);
	void SetRotationW(XMVECTOR Quatanion);
	//位置の取得
	CXMVECTOR GetPos();
	//前方ベクトル取得
	CXMVECTOR GetFlont();

	//ワールド行列取得
	XMMATRIX GetWorldMat();
	//ローカルビュー行列取得
	XMMATRIX GetViewMatL();
	//ワールドビュー行列取得
	XMMATRIX GetViewMatW();
	//プロジェクション行列取得
	XMMATRIX GetProjMat();
};

