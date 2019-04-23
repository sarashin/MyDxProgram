#include "stdafx.h"
#include "MeshObjectFactory.h"

using namespace std;
using namespace fbxsdk;

//コンストラクタ
MeshFactory::MeshFactory()
{
	m_pTexture = make_unique<TextureStocker>();
}

//デストラクタ
MeshFactory::~MeshFactory()
{
	Release();
}

//初期化
void MeshFactory::Init(shared_ptr<D3DRenderer> pDevice)
{
	//変数初期化
	m_pFbxManager = FbxManager::Create();
	m_pIOSettings = FbxIOSettings::Create(m_pFbxManager, "");
	m_pDevice = pDevice;
}

//メッシュ作成
bool MeshFactory::CreateMeshObject(const char* FName, shared_ptr<vector<MyMeshData>> pMeshData, shared_ptr<MyAnimationContainer> pAnimContainer, shared_ptr<MySkelton> pSkelton)
{
	if (pMeshData == NULL)
		return false;

	m_pImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxManager->SetIOSettings(m_pIOSettings);

	int FileFormat = -1;

	if (!m_pImporter->Initialize(FName,FileFormat,m_pFbxManager->GetIOSettings()))
	{
		//ファイルが無いまたは対応バージョンでないなら終了
		return false;
	}

	//ファイルを開く
	//if (FileFormat==-1)
	//{
	//	// 失敗
	//	return false;
	//}
	//シーンオブジェクト作成
	FbxScene* pFbxScene = FbxScene::Create(m_pFbxManager, FName);
	m_pImporter->Import(pFbxScene);
	m_pImporter->Destroy();
	
	FbxGeometryConverter Converter(m_pFbxManager);
	Converter.Triangulate(pFbxScene,true);
	Converter.RemoveBadPolygonsFromMeshes(pFbxScene);
	
	///読み込み開始-----------------------------------------------------

	int MeshNum = pFbxScene->GetSrcObjectCount<FbxMesh>();	//メッシュ数
	
	vector<MyVertex> VertexArray;							//頂点配列
	MyLambert Lambert;										//マテリアル
	vector<ComPtr<ID3D11ShaderResourceView>> pTex;			//テクスチャ
	vector<MyMeshData>*	pMyMesh;							//メッシュの生ポインタ
	vector<MyMesh>		Mesh;								//メッシュ配列
	vector<RefBone>		Bone;								//頂点のボーンによる影響
	
	ID3D11Device* pDevice = m_pDevice->GetDevice();			//デバイス

	vector<MySkinAnimation> Animation;						//アニメーション情報

	pMyMesh = pMeshData.get();

	//ファイルに含まれるメッシュ数
	for (int i = 0; i < MeshNum; i++)
	{
		FbxMesh* pMesh = pFbxScene->GetSrcObject<FbxMesh>(i);
		//ポリゴングループ作成
		CreatePolyGroup(pMesh, Mesh);
	
		int MaterialNum = pMesh->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
		int MeshSize = pMyMesh->size();
		pMyMesh->resize(MeshSize+MaterialNum);
		Bone.resize(Mesh[Mesh.size()-1].VertexCount);

		if(pAnimContainer!=NULL)
			CreateAnimInfo(pMesh, Bone, Animation,pSkelton);

		if(pSkelton!=NULL)
			pSkelton->CreateBoneBuffer(m_pDevice->GetDevice());

		
		//マテリアルとメッシュの作成
		for (int j = MeshSize; j < MeshSize+MaterialNum; j++)
		{
			//マテリアル
			CreateMaterial(pMesh, Lambert, pTex, j);
			Mesh[j].m_pRefBone = make_shared<vector<RefBone>>(Bone);

			
			(*pMyMesh)[j] = MyMeshData(Mesh[j], Lambert, pTex);

			//デフォルトシェーダを設定
			(*pMyMesh)[j].SetVShader(m_pDevice->m_pDefVShader);
			(*pMyMesh)[j].SetPShader(m_pDevice->m_pDefPShader);

			(*pMyMesh)[j].CreateVertBuffer(pDevice);
			(*pMyMesh)[j].CreateIndexBuffer(pDevice);
			(*pMyMesh)[j].CreateMaterialBuffer(pDevice);

			pTex.clear();
		}
	}

	pAnimContainer = make_unique<MyAnimationContainer>(Animation);
	///読み込み終了----------------------------------------------------
	
	pMyMesh = NULL;
	pTex.clear();

	return true;
}

//XMMATRIXへ型変換
void CastMatrix(XMMATRIX* Mat, FbxMatrix FbxMat)
{
	Mat->m[0][0] = (FLOAT)FbxMat.mData[0][0];
	Mat->m[0][1] = (FLOAT)FbxMat.mData[0][1];
	Mat->m[0][2] = (FLOAT)FbxMat.mData[0][2];
	Mat->m[0][3] = (FLOAT)FbxMat.mData[0][3];
	Mat->m[1][0] = (FLOAT)FbxMat.mData[1][0];
	Mat->m[1][1] = (FLOAT)FbxMat.mData[1][1];
	Mat->m[1][2] = (FLOAT)FbxMat.mData[1][2];
	Mat->m[1][3] = (FLOAT)FbxMat.mData[1][3];
	Mat->m[2][0] = (FLOAT)FbxMat.mData[2][0];
	Mat->m[2][1] = (FLOAT)FbxMat.mData[2][1];
	Mat->m[2][2] = (FLOAT)FbxMat.mData[2][2];
	Mat->m[2][3] = (FLOAT)FbxMat.mData[2][3];
	Mat->m[3][0] = (FLOAT)FbxMat.mData[3][0];
	Mat->m[3][1] = (FLOAT)FbxMat.mData[3][1];
	Mat->m[3][2] = (FLOAT)FbxMat.mData[3][2];
	Mat->m[3][3] = (FLOAT)FbxMat.mData[3][3];
}

//頂点情報取得
void MeshFactory::SetVertex(FbxMesh* pMesh, vector<XMFLOAT4>& pVertex)
{
	int VertexCount = pMesh->GetControlPointsCount();	//頂点数
	FbxVector4* Vertices = pMesh->GetControlPoints();	//頂点インデックス配列
	vector<XMFLOAT4> m_pVertex;							//頂点情報
	int i;
	m_pVertex.resize(VertexCount);

	for (i = 0; i < VertexCount; ++i)					//頂点座標取得 
	{
		m_pVertex[i].x = (FLOAT)Vertices[i][0];
		m_pVertex[i].y = (FLOAT)Vertices[i][1];
		m_pVertex[i].z = (FLOAT)Vertices[i][2];
		m_pVertex[i].w = (FLOAT)Vertices[i][3];
	}

	pVertex = m_pVertex;
}

//ポリゴンのインデックス取得
void MeshFactory::SetIndex(FbxMesh* pMesh, vector<int>& pIndex)
{
	if (pMesh == NULL)
	{
		return;
	}

	int PolygonNum = pMesh->GetPolygonCount();					//ポリゴン数
	int PolygonVertexNum = pMesh->GetPolygonVertexCount();		//ポリゴンの頂点インデックス数
	int* Vertics = pMesh->GetPolygonVertices();					//ポリゴンの頂点インデックス配列
	vector<int> Poly;
	Poly.resize(PolygonNum * 3);
	

	int PolyInMaterial = pMesh->GetPolygonCount();
	int PolyNum=0;
	
	for (int i = 0; i < PolyInMaterial;i++)
	{
		int IndexNumInPoly = pMesh->GetPolygonSize(i);			//i番目のポリゴンの頂点数

		for (int j = 0; j < IndexNumInPoly; j++)
		{
			Poly[PolyNum + j] = pMesh->GetPolygonVertex(i, j);	//i番目のポリゴンのj番目の頂点のインデックス番号
		}

		PolyNum += IndexNumInPoly;
	}
	
	pIndex = Poly;
}

//法線取得
void MeshFactory::SetNormal(FbxLayer* Layer ,vector<XMFLOAT3>& pNormal)
{
	vector<XMFLOAT3> Normal;									//法線データの取得

	FbxLayerElementNormal* NormalElem = Layer->GetNormals();	//法線情報
	if (NormalElem == NULL)
	{
		return;   // 法線無し
	}

	int    NormalNum = NormalElem->GetDirectArray().GetCount();	//法線の数
	int    IndexNum = NormalElem->GetIndexArray().GetCount();	//格納されているインデックス数
	Normal.resize(NormalNum);

	FbxLayerElement::EMappingMode   NormalMapMode = NormalElem->GetMappingMode();//マッピングモード
	
	FbxLayerElement::EReferenceMode NormalRefMode = NormalElem->GetReferenceMode();//取得方法

	if (NormalMapMode == FbxLayerElement::eByPolygonVertex)
	{
		if (NormalRefMode == FbxLayerElement::eDirect)
		{
			for (int i = 0; i < NormalNum; ++i) {	// 直接取得
				Normal[i].x = (float)NormalElem->GetDirectArray().GetAt(i)[0];
				Normal[i].y = (float)NormalElem->GetDirectArray().GetAt(i)[1];
				Normal[i].z = (float)NormalElem->GetDirectArray().GetAt(i)[2];
			}
		}
	}
	else if (NormalMapMode == FbxLayerElement::eByControlPoint)
	{
		if (NormalRefMode == FbxLayerElement::eDirect)
		{
			for (int i = 0; i < NormalNum; ++i) {	// 直接取得
				Normal[i].x = (float)NormalElem->GetDirectArray().GetAt(i)[0];
				Normal[i].y = (float)NormalElem->GetDirectArray().GetAt(i)[1];
				Normal[i].z = (float)NormalElem->GetDirectArray().GetAt(i)[2];
			}
		}
	}

	pNormal = Normal;
}

void MeshFactory::SetUV(FbxLayer* pLayer, vector<XMFLOAT2>& pUV)
{
	FbxLayerElementUV* UVElem = pLayer->GetUVs();				//UV取得

	int UVCount = UVElem->GetDirectArray().GetCount();			//UV座標の数
	int UVindexCount = UVElem->GetIndexArray().GetCount();		//UVインデックスの数
	int Size = UVCount > UVindexCount ? UVCount : UVindexCount;	//上二つの大きい方をサイズに
	vector<XMFLOAT2> UV;
	UV.resize(Size);
	vector<int> s;
	
	FbxLayerElement::EMappingMode UVMapMode = UVElem->GetMappingMode();	//マッピングモード
	FbxLayerElement::EReferenceMode RefMode = UVElem->GetReferenceMode();//取得方法

	if (UVMapMode == FbxLayerElement::eByPolygonVertex)
	{
		FbxArray<FbxVector2> UVArray;
		UVElem->GetDirectArray().CopyTo(UVArray);

		if (RefMode == FbxLayerElement::eDirect)
		{
			for (int j = 0; j < Size; ++j) {		// 直接取得
				UV[j].x = (float)UVArray.GetAt(j)[0];
				UV[j].y = (float)UVArray.GetAt(j)[1];
			}
		}
		else if (RefMode == FbxLayerElement::eIndexToDirect)
		{
			int index;

			for (int j = 0; j <Size; ++j){			// インデックスから取得
				index = UVElem->GetIndexArray().GetAt(j);
				s.push_back(index);
				UV[j].x = (float)UVArray.GetAt(index)[0];
				UV[j].y = 1.0-(float)UVArray.GetAt(index)[1];
			}
		}
	}

	pUV = UV;
	UVElem = NULL;
}

//メッシュの取得
void MeshFactory::CreateVertices(FbxMesh* pMesh, vector<MyVertex>& pVertex)
{
	int PolygonNum = pMesh->GetPolygonCount();			//ポリゴン数
	int VertexCount = pMesh->GetControlPointsCount();	//頂点数
	if (!(PolygonNum > 1))
		return;
	
	vector<XMFLOAT4>	m_pVertex;		//頂点座標
	vector<int>			Poly;		//ポリゴンインデックス
	vector<XMFLOAT3>	Normal;		//法線ベクトル
	vector<XMFLOAT2>	UV;			//UV座標

	int LayerNum = pMesh->GetLayerCount();
	const char* Name=pMesh->GetName();
	FbxLayer* Layer = pMesh->GetLayer(0);
	int PolygonVertexNum = pMesh->GetPolygonVertexCount();
	
	Poly.resize(PolygonVertexNum);
	Normal.resize(PolygonVertexNum);

	SetVertex(pMesh, m_pVertex);//頂点取得
	SetIndex(pMesh, Poly);		//インデックス取得

	//UVと法線はインデックスでソート
	SetNormal(Layer, Normal);	//法線取得
	//ソート
	vector<XMFLOAT3> SortNorm;
	SortNorm.resize(VertexCount);
	for (int i = 0; i < Normal.size(); i++)
		SortNorm[Poly[i]] = Normal[i];

	SetUV(Layer, UV);			//UV座標取得
	
	vector<XMFLOAT2> SortUV;
	SortUV.resize(VertexCount);
	for (int i = 0; i < UV.size(); i++)//ソート
		SortUV[Poly[i]] = UV[i];

	pVertex.resize(VertexCount);
	
	for (int i = 0; i < VertexCount; i++)//データ格納
	{
		pVertex[i].Position = m_pVertex[i];
		pVertex[i].Normal	= SortNorm[i];
		pVertex[i].UV		= SortUV[i];
	}

	m_pVertex.clear();
	Poly.clear();
	Normal.clear();
	UV.clear();
}

void MeshFactory::CreatePolyGroup(FbxMesh* pMesh, vector<MyMesh>& Vertices)
{
	int MaterialNum = pMesh->GetNode()->GetMaterialCount();						//マテリアル数
	
	if (MaterialNum < 0)
		return;

	int PolygonNum;																//ポリゴン数
	vector<MyVertex> pVertex;	//頂点配列
	FbxLayerElementArrayTemplate<int>* MaterialIndex;

	CreateVertices(pMesh, pVertex);											//頂点配列は参照するため1つだけ作る

	int* Vertics = pMesh->GetPolygonVertices();									//ポリゴンの頂点インデックス配列
	int VertexEnd = Vertices.size();											//取得する頂点の初めのインデックス数
	vector<vector<int>> PolygonIndex;
	PolygonIndex.resize(MaterialNum);
	Vertices.resize(Vertices.size() + MaterialNum);

	if (MaterialNum > 1)	//マテリアル数で取得方法変更
	{
		pMesh->GetMaterialIndices(&MaterialIndex);//インデックスで分ける
		PolygonNum = MaterialIndex->GetCount();
		vector<int> Index;

		for (int i = 0; i < PolygonNum; i++)
		{
			int PolygonVertexNum = pMesh->GetPolygonSize(i);

			for (int j = 0; j < PolygonVertexNum; j++)
				PolygonIndex[MaterialIndex->GetAt(i)].push_back(pMesh->GetPolygonVertex(i, j));
		}
	}
	else   //マテリアル数1以下
	{
		PolygonNum = pMesh->GetPolygonCount();
		
		vector<int> Index;

		for (int i = 0; i < PolygonNum; i++)
		{
			int PolygonVertexNum = pMesh->GetPolygonSize(i);

			for (int j = 0; j<PolygonVertexNum; j++)
				Index.push_back(pMesh->GetPolygonVertex(i,j));
		}

		PolygonIndex[0] = Index;
	}

	for (int i = VertexEnd; i < MaterialNum+VertexEnd; i++)//情報格納
	{
		Vertices[i] = MyMesh(pVertex, PolygonIndex[i]);
	}
}

//ランバートマテリアル
void SetLambertInfo(FbxSurfaceLambert* Lambert, MyLambert* Material)
{
	//アンビエントカラー
	Material->Ambient.x = Lambert->Ambient.Get()[0];
	Material->Ambient.y = Lambert->Ambient.Get()[1];
	Material->Ambient.z = Lambert->Ambient.Get()[2];
	Material->Ambient.w = 1.0;

	//ディフューズ
	Material->Diffuse.x = Lambert->Diffuse.Get()[0];
	Material->Diffuse.y = Lambert->Diffuse.Get()[1];
	Material->Diffuse.z = Lambert->Diffuse.Get()[2];

	//透過
	Material->Alpha = 1.0f;
}

//phongマテリアル
void SetPhongInfo(FbxSurfacePhong* Phong, MyPhong* Material)
{
	//ランバートと同じ情報を取得
	SetLambertInfo(Phong, Material);

	//スペキュラ
	Material->Specular.x = Phong->Specular.Get()[0];
	Material->Specular.y = Phong->Specular.Get()[1];
	Material->Specular.z = Phong->Specular.Get()[2];
	//光沢
	Material->Shiness = Phong->Shininess.Get();
	//反射光
	Material->Refrect = Phong->ReflectionFactor.Get();
}

//マテリアル作成
void MeshFactory::CreateMaterial(FbxMesh* pMesh, MyLambert& MaterialData, vector<ComPtr<ID3D11ShaderResourceView>>& pTex,int MeshNum)
{
	FbxNode* Node = pMesh->GetNode();			//メッシュのノード取得

	if (Node == NULL)
	{
		return;
	}

	int MaterialNum = Node->GetMaterialCount();	//マテリアル数
	if (MaterialNum == 0)
	{
		return;
	}
	//マテリアル格納
	MyLambert	DummyMaterial;

	FbxSurfaceMaterial* Material = Node->GetMaterial(MeshNum);
	
	if (Material == NULL)
	{
		return;
	}

	if (Material->GetClassId().Is(FbxSurfaceLambert::ClassId))		//ランバートマテリアル
	{
		FbxSurfaceLambert* Lambert = (FbxSurfaceLambert*)Material;
		SetLambertInfo(Lambert, &MaterialData);
	}
	else if (Material->GetClassId().Is(FbxSurfacePhong::ClassId))	//フォングマテリアル
	{
		FbxSurfacePhong* Phong = (FbxSurfacePhong*)Material;
	}
	else
	{
		SetLambertInfo(NULL, &MaterialData);
	}

	FbxProperty DiffuseProperty = Material->FindProperty(FbxSurfaceMaterial::sDiffuse);//マテリアルのプロパティ
	int layerNum = DiffuseProperty.GetSrcObjectCount(FbxCriteria::ObjectType(FbxLayeredTexture::ClassId));	//レイヤ数

	//テクスチャ取得
	if (layerNum > 0)
	{
		//レイヤードテクスチャ
	}
	else
	{
		//テクスチャ一つ
		int TexNum = DiffuseProperty.GetSrcObjectCount(FbxCriteria::ObjectType(FbxTexture::ClassId));
		if (TexNum == 0)
			return;

		FbxFileTexture* FileTex = FbxCast<FbxFileTexture>(DiffuseProperty.GetSrcObject(FbxCriteria::ObjectType(FbxTexture::ClassId),0));
		if (FileTex == NULL)
			return;
		
		FbxString Path = FbxPathUtils::GetFileName(FileTex->GetFileName());//ファイルパス
		const char* TexPath = Path;

		char RelativePath[255] = "../ShaderPractice/Resource/";	
		strcat_s(RelativePath, 255, TexPath);//相対パスへ変換

		
		auto It = m_pTexture->Textures.find(TexPath);

		if (It == m_pTexture->Textures.end())
		{
			//パスからテクスチャを取得
			HRESULT hr;
			ID3D11ShaderResourceView* Tex;
			hr = D3DX11CreateShaderResourceViewFromFileA(m_pDevice->GetDevice(), RelativePath, NULL, NULL, &Tex, NULL);

			if (FAILED(hr))
			{
				TexNum = 0;
				return;
			}
			m_pTexture->Textures[TexPath] = Tex;
			pTex.push_back(Tex);
		}
		else
		{
			pTex.push_back(It->second);		//既に同じ名前のテクスチャがあるなら追加しないで参照
		}
	}
}

//アニメーション取得
void MeshFactory::CreateAnimInfo(FbxMesh* pMesh,vector<RefBone>& pBone, vector<MySkinAnimation>& pAnim, shared_ptr<MySkelton> pSkelton)
{
	FbxNode* pNode = pMesh->GetNode();		//ノード
	FbxScene* AnimScene = pMesh->GetScene();//シーン

	FbxTime::EMode TimeMode;				//タイムモード取得
	TimeMode = AnimScene->GetGlobalSettings().GetTimeMode();

	FbxTime Period;							//1フレームの時間
	Period.SetTime(0, 0, 0, 1, 0, TimeMode);

	FbxArray<FbxString*> NameArray;			//アニメーション名
	AnimScene->FillAnimStackNameArray(NameArray);
	int AnimNum = NameArray.GetCount();		//アニメーション数
	pAnim.resize(AnimNum);

	FbxTakeInfo* TakeInfo = new FbxTakeInfo;//テイクの情報
	FbxTime Start;							//アニメーションスタート時間
	FbxTime Stop;							//終了時間
	int startFrame;							//アニメーションスタートフレーム
	int stopFrame;							//終了フレーム
	FbxMatrix MatrixData;					//姿勢

	for (int i = 0; i < AnimNum; i++)
	{
		TakeInfo = AnimScene->GetTakeInfo(*(NameArray[i]));
		if (TakeInfo)
		{
			//continue;
		}

		//1テイクの開始、終了時間取得
		Start = TakeInfo->mLocalTimeSpan.GetStart();
		Stop  = TakeInfo->mLocalTimeSpan.GetStop();
		pAnim[i].Start = (int)(Start.Get() / Period.Get());
		pAnim[i].Stop = (int)(Stop.Get() / Period.Get());

		pAnim[i].m_AnimMatrix.resize(pAnim[i].Stop - pAnim[i].Start);

		//ボーンの情報取得
		CreateBoneInfo(pMesh, pBone,pAnim[i],pSkelton, pAnim[i].Start, pAnim[i].Stop);

		////アニメーションコントローラにセット
		string Name = NameArray[i]->Buffer();
		pAnim[i].Name = Name;
	}

	TakeInfo = NULL;
	NameArray = NULL;
}

void SetVertexRefBone(FbxCluster* pCluster,vector<RefBone>& pBone, int ClusterNum)
{
	RefBone Bone;
	int CPCount = pCluster->GetControlPointIndicesCount();//クラスタの対応頂点数

	if (CPCount == 0)
		return;

	int* Vertices = pCluster->GetControlPointIndices();//クラスタの対応頂点
	double* Weights =pCluster->GetControlPointWeights();//対応頂点のウエイト

	for (int i = 0; i < CPCount; i++)
	{
		pBone[Vertices[i]].BoneId.push_back(ClusterNum);
		pBone[Vertices[i]].Weight.push_back((float)Weights[i]);
	}

	Vertices = NULL;
	Weights = NULL;
}

void SetBone(FbxCluster* pCluster, MyBone& pBone)
{
	FbxAMatrix  InitMat;	//初期姿勢						
	XMMATRIX	CastMat;	//変換用姿勢
	
	InitMat=pCluster->GetTransformMatrix(InitMat).Inverse();
	CastMatrix(&pBone.m_InitMat, InitMat);
}

void SetAnim(FbxCluster* pCluster, MySkinAnimation& pAnim, int ClusterNum, int StartFrame, int StopFrame)
{
	int CPCount = pCluster->GetControlPointIndicesCount();//クラスタの対応頂点数

	if (CPCount == 0)
		return;

	FbxAMatrix  InitMat;	//初期姿勢						
	FbxMatrix	FrameMat;	//フレーム姿勢
	XMMATRIX	CastMat;	//変換用姿勢

	pAnim.m_AnimBoneMatrix[ClusterNum].reserve(StopFrame - StartFrame);
	FbxNode* pNode = pCluster->GetLink();//アニメーションのノード
	FbxTime Time;

	for (int i = 0; i < StopFrame-StartFrame; i++)//各フレームの姿勢取得
	{
		Time.SetSecondDouble(i);
		FrameMat = pNode->EvaluateGlobalTransform(StopFrame).Inverse();
		CastMatrix(&CastMat, FrameMat);
		pAnim.m_AnimBoneMatrix[ClusterNum].push_back(CastMat);
	}
}

//ボーンの情報取得
void MeshFactory::CreateBoneInfo(FbxMesh* pMesh, vector<RefBone>& pRefBone,MySkinAnimation& pAnim, shared_ptr<MySkelton> pSkelton, int StartFrame,int StopFrame)
{
	int SkinCount= pMesh->GetDeformerCount(FbxDeformer::eSkin);	//スキン数
	FbxSkin* pSkin;												//スキン
	int ClusterNum = 0;											//クラスタ数
	FbxCluster* pCluster;										//クラスタ
	
	for (int i = 0; i < SkinCount; i++)
	{
		pSkin = (FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);//スキンを取得
		ClusterNum += pSkin->GetClusterCount();
		pSkelton->m_Bone.resize(ClusterNum);
		pAnim.m_AnimBoneMatrix.resize(ClusterNum);

		for (int j = 0; j < ClusterNum; j++)		//ボーンのとアニメーションを取得
		{
			pCluster = pSkin->GetCluster(j);

			SetBone(pCluster, pSkelton->m_Bone[j]);
			SetVertexRefBone(pCluster, pRefBone, j);
			SetAnim(pCluster, pAnim, j, StartFrame, StopFrame);
		}
	}

	pSkin = NULL;
	pCluster = NULL;
}

//スプライト作成
bool MeshFactory::CreateSprite(const char* FileName, shared_ptr<Sprite>& pSprite, RECT Rect, RECT Size)
{
	MyMesh Mesh;
	//スプライトの頂点定義
	int VertexCount = 4;
	vector<MyVertex> Vertex;
	Vertex.resize(VertexCount);
	Vertex[0].Position = XMFLOAT4(Size.left, Size.top, 0, 0);
	Vertex[0].Normal = XMFLOAT3(0, -1, 0);
	Vertex[0].UV= XMFLOAT2(Rect.left, Rect.top);
	Vertex[1].Position = XMFLOAT4(Size.right, Size.top, 0, 0);
	Vertex[1].Normal = XMFLOAT3(0, -1, 0);
	Vertex[1].UV = XMFLOAT2(Rect.right, Rect.top);
	Vertex[2].Position = XMFLOAT4(Size.right, Size.bottom, 0, 0);
	Vertex[2].Normal = XMFLOAT3(0, -1, 0);
	Vertex[2].UV = XMFLOAT2(Size.right, Size.bottom);
	Vertex[3].Position = XMFLOAT4(Size.left, Size.bottom, 0, 0);
	Vertex[3].Normal = XMFLOAT3(0, -1, 0);
	Vertex[3].UV = XMFLOAT2(Size.left, Size.bottom);

	//スプライトの頂点インデックス定義
	int PolygonCount = 6;
	vector<int> Index;
	Index.resize(6);
	Index[0] = 0;
	Index[1] = 1;
	Index[2] = 3;
	Index[3] = 1;
	Index[4] = 2;
	Index[5] = 3;


	//テクスチャ
	ID3D11ShaderResourceView* Tex;
	char RelativePath[255] = "../ShaderPractice/Resource/";
	strcat_s(RelativePath, 255, FileName);//相対パスへ変換

	D3DX11CreateShaderResourceViewFromFile(m_pDevice->GetDevice(), RelativePath, NULL, NULL, &Tex, NULL);

	Mesh = MyMesh(Vertex, Index);
	pSprite = make_shared<Sprite>(Mesh, Tex);
	pSprite->m_Material.Alpha = 1.0;

	//バッファ作成
	pSprite->CreateVertBuffer(m_pDevice->GetDevice());
	pSprite->CreateIndexBuffer(m_pDevice->GetDevice());
	pSprite->CreateMaterialBuffer(m_pDevice->GetDevice());

	//テクスチャサンプラ
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	m_pDevice->GetDevice()->CreateSamplerState(&SamDesc, &pSprite->m_pSampleLiner);

	return true;
}

//解放
void MeshFactory::Release()
{
	if (m_pTexture != NULL)
	{
		m_pTexture.reset();
	}

	if (m_pFbxManager != NULL)
	{
		m_pFbxManager->Destroy();
		m_pFbxManager = NULL;
	}
}