#pragma once
using namespace Microsoft::WRL;

//ボーン
class MyBone
{
public:
	//姿勢
	XMMATRIX	m_Matrix;	
	//初期姿勢
	XMMATRIX	m_InitMat;	
	//親ボーンのポインタ
	vector<shared_ptr<MyBone>> pParent;
	//子ボーンのポインタ
	vector<shared_ptr<MyBone>> pChild;
	float BoneId;
};

//シェーダへ渡すボーン
class RefBone
{
public:
	//重み
	vector<float> Weight;
	//ボーンid
	vector<float> BoneId;
};

//ボーンの集合
class MySkelton
{
public:
	//ボーン情報
	vector<MyBone> m_Bone;				
	//シェーダへ渡すボーン配列
	ComPtr<ID3D11Buffer> m_BoneBuffer;	

	//コンストラクタ
	MySkelton();
	//コンストラクタ
	//Bones:ボーン情報
	MySkelton(vector<MyBone> Bones);
	//デストラクタ
	~MySkelton();

	//ボーン数取得
	int					GetBoneNum();
	//シェーダへ渡すボーン情報
	void CreateBoneBuffer(ComPtr<ID3D11Device> pDevice);
};

//シェーダへ渡すボーン配列
struct BoneMatBuf
{
	vector<XMMATRIX> BoneMat;
};