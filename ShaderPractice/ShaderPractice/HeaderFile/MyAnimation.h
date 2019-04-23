#pragma once
#include"Mesh.h"
#include"MySkelton.h"

using namespace fbxsdk;
using namespace Microsoft::WRL;
using namespace std;


//アニメーション
class MySkinAnimation
{
public:
	//コンストラクタ
	MySkinAnimation();
	//デストラクタ
	~MySkinAnimation();

	//各フレームの姿勢(主ボーン)
	vector<XMMATRIX>		 m_AnimMatrix;	  
	//各フレームの姿勢(その他ボーン)
	vector<vector<XMMATRIX>> m_AnimBoneMatrix;

	//開始フレーム
	int Start;						
	//終了フレーム
	int Stop;						

	//名前
	string Name;					

	int GetBoneNum();
	void Release();
};

//アニメーションコンテナ
class MyAnimationContainer
{
	//アニメーションコンテナ
	shared_ptr<vector<MySkinAnimation>> m_pAnimation;	
	//インデックス
	map<string,int>			m_AnimIndex;				
	//メッシュへのポインタ
	shared_ptr<MyMeshData>  m_pMesh;					
	//現アニメーションのインデックスナンバー
	int						NowAnimNum;					
	//名前
	string					NowAnimName;				
public:
	//コンストラクタ
	MyAnimationContainer();
	//コンストラクタ
	//Animation:アニメーション情報
	MyAnimationContainer(vector<MySkinAnimation> Animation);

	//アニメーション更新
	void UpdateAnimation(int AniIndex,float Frame, MySkelton& pSkelton);
};