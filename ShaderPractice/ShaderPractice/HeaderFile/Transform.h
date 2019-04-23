#pragma once
#include <xnamath.h>

class Transform
{
public:
	//姿勢
	XMFLOAT4X4 TransformMatrix;
	
	//コンストラクタ
	Transform();

	//コンストラクタ
	//Pos:初期座標
	Transform(XMFLOAT3 Pos);

	//コンストラクタ
	//Mat:初期姿勢
	Transform(CXMMATRIX Mat);

	//デストラクタ
	~Transform();

	void SetMat(CXMMATRIX Matrix);		//姿勢のセット
	XMMATRIX GetMat();					//姿勢取得
	void SetPosition(XMVECTOR Pos);		//位置のセット
	void SetPosition(float x, float y, float z);	//位置のセット
	void MovePosition(XMVECTOR Pos);	//位置を動かす
	void MovePosition(float x,float y,float z);	//位置を動かす
	XMVECTOR GetPosition();				//位置の取得
	void SetRotation(XMVECTOR Rotation);//回転のセット
	void SetRotation(CXMMATRIX RotationMat);
	void RotationQuatanion(XMVECTOR Rot);//任意軸回転
	void Rotation(float x, float y, float z);
	XMVECTOR GetRotationQuatanion();	//回転角度取得
	void AddScale(float Scale);			//スケールの拡縮
	XMVECTOR GetScale();				//スケールの取得

	//前方ベクトル取得
	XMFLOAT3 GetFlont();
	//右ベクトル取得
	XMFLOAT3 GetRight();
};

