#include "stdafx.h"
#include "MeshObjectFactory.h"

using namespace std;
using namespace fbxsdk;

//�R���X�g���N�^
MeshFactory::MeshFactory()
{
	m_pTexture = make_unique<TextureStocker>();
}

//�f�X�g���N�^
MeshFactory::~MeshFactory()
{
	Release();
}

//������
void MeshFactory::Init(shared_ptr<D3DRenderer> pDevice)
{
	//�ϐ�������
	m_pFbxManager = FbxManager::Create();
	m_pIOSettings = FbxIOSettings::Create(m_pFbxManager, "");
	m_pDevice = pDevice;
}

//���b�V���쐬
bool MeshFactory::CreateMeshObject(const char* FName, shared_ptr<vector<MyMeshData>> pMeshData, shared_ptr<MyAnimationContainer> pAnimContainer, shared_ptr<MySkelton> pSkelton)
{
	if (pMeshData == NULL)
		return false;

	m_pImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxManager->SetIOSettings(m_pIOSettings);

	int FileFormat = -1;

	if (!m_pImporter->Initialize(FName,FileFormat,m_pFbxManager->GetIOSettings()))
	{
		//�t�@�C���������܂��͑Ή��o�[�W�����łȂ��Ȃ�I��
		return false;
	}

	//�t�@�C�����J��
	//if (FileFormat==-1)
	//{
	//	// ���s
	//	return false;
	//}
	//�V�[���I�u�W�F�N�g�쐬
	FbxScene* pFbxScene = FbxScene::Create(m_pFbxManager, FName);
	m_pImporter->Import(pFbxScene);
	m_pImporter->Destroy();
	
	FbxGeometryConverter Converter(m_pFbxManager);
	Converter.Triangulate(pFbxScene,true);
	Converter.RemoveBadPolygonsFromMeshes(pFbxScene);
	
	///�ǂݍ��݊J�n-----------------------------------------------------

	int MeshNum = pFbxScene->GetSrcObjectCount<FbxMesh>();	//���b�V����
	
	vector<MyVertex> VertexArray;							//���_�z��
	MyLambert Lambert;										//�}�e���A��
	vector<ComPtr<ID3D11ShaderResourceView>> pTex;			//�e�N�X�`��
	vector<MyMeshData>*	pMyMesh;							//���b�V���̐��|�C���^
	vector<MyMesh>		Mesh;								//���b�V���z��
	vector<RefBone>		Bone;								//���_�̃{�[���ɂ��e��
	
	ID3D11Device* pDevice = m_pDevice->GetDevice();			//�f�o�C�X

	vector<MySkinAnimation> Animation;						//�A�j���[�V�������

	pMyMesh = pMeshData.get();

	//�t�@�C���Ɋ܂܂�郁�b�V����
	for (int i = 0; i < MeshNum; i++)
	{
		FbxMesh* pMesh = pFbxScene->GetSrcObject<FbxMesh>(i);
		//�|���S���O���[�v�쐬
		CreatePolyGroup(pMesh, Mesh);
	
		int MaterialNum = pMesh->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
		int MeshSize = pMyMesh->size();
		pMyMesh->resize(MeshSize+MaterialNum);
		Bone.resize(Mesh[Mesh.size()-1].VertexCount);

		if(pAnimContainer!=NULL)
			CreateAnimInfo(pMesh, Bone, Animation,pSkelton);

		if(pSkelton!=NULL)
			pSkelton->CreateBoneBuffer(m_pDevice->GetDevice());

		
		//�}�e���A���ƃ��b�V���̍쐬
		for (int j = MeshSize; j < MeshSize+MaterialNum; j++)
		{
			//�}�e���A��
			CreateMaterial(pMesh, Lambert, pTex, j);
			Mesh[j].m_pRefBone = make_shared<vector<RefBone>>(Bone);

			
			(*pMyMesh)[j] = MyMeshData(Mesh[j], Lambert, pTex);

			//�f�t�H���g�V�F�[�_��ݒ�
			(*pMyMesh)[j].SetVShader(m_pDevice->m_pDefVShader);
			(*pMyMesh)[j].SetPShader(m_pDevice->m_pDefPShader);

			(*pMyMesh)[j].CreateVertBuffer(pDevice);
			(*pMyMesh)[j].CreateIndexBuffer(pDevice);
			(*pMyMesh)[j].CreateMaterialBuffer(pDevice);

			pTex.clear();
		}
	}

	pAnimContainer = make_unique<MyAnimationContainer>(Animation);
	///�ǂݍ��ݏI��----------------------------------------------------
	
	pMyMesh = NULL;
	pTex.clear();

	return true;
}

//XMMATRIX�֌^�ϊ�
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

//���_���擾
void MeshFactory::SetVertex(FbxMesh* pMesh, vector<XMFLOAT4>& pVertex)
{
	int VertexCount = pMesh->GetControlPointsCount();	//���_��
	FbxVector4* Vertices = pMesh->GetControlPoints();	//���_�C���f�b�N�X�z��
	vector<XMFLOAT4> m_pVertex;							//���_���
	int i;
	m_pVertex.resize(VertexCount);

	for (i = 0; i < VertexCount; ++i)					//���_���W�擾 
	{
		m_pVertex[i].x = (FLOAT)Vertices[i][0];
		m_pVertex[i].y = (FLOAT)Vertices[i][1];
		m_pVertex[i].z = (FLOAT)Vertices[i][2];
		m_pVertex[i].w = (FLOAT)Vertices[i][3];
	}

	pVertex = m_pVertex;
}

//�|���S���̃C���f�b�N�X�擾
void MeshFactory::SetIndex(FbxMesh* pMesh, vector<int>& pIndex)
{
	if (pMesh == NULL)
	{
		return;
	}

	int PolygonNum = pMesh->GetPolygonCount();					//�|���S����
	int PolygonVertexNum = pMesh->GetPolygonVertexCount();		//�|���S���̒��_�C���f�b�N�X��
	int* Vertics = pMesh->GetPolygonVertices();					//�|���S���̒��_�C���f�b�N�X�z��
	vector<int> Poly;
	Poly.resize(PolygonNum * 3);
	

	int PolyInMaterial = pMesh->GetPolygonCount();
	int PolyNum=0;
	
	for (int i = 0; i < PolyInMaterial;i++)
	{
		int IndexNumInPoly = pMesh->GetPolygonSize(i);			//i�Ԗڂ̃|���S���̒��_��

		for (int j = 0; j < IndexNumInPoly; j++)
		{
			Poly[PolyNum + j] = pMesh->GetPolygonVertex(i, j);	//i�Ԗڂ̃|���S����j�Ԗڂ̒��_�̃C���f�b�N�X�ԍ�
		}

		PolyNum += IndexNumInPoly;
	}
	
	pIndex = Poly;
}

//�@���擾
void MeshFactory::SetNormal(FbxLayer* Layer ,vector<XMFLOAT3>& pNormal)
{
	vector<XMFLOAT3> Normal;									//�@���f�[�^�̎擾

	FbxLayerElementNormal* NormalElem = Layer->GetNormals();	//�@�����
	if (NormalElem == NULL)
	{
		return;   // �@������
	}

	int    NormalNum = NormalElem->GetDirectArray().GetCount();	//�@���̐�
	int    IndexNum = NormalElem->GetIndexArray().GetCount();	//�i�[����Ă���C���f�b�N�X��
	Normal.resize(NormalNum);

	FbxLayerElement::EMappingMode   NormalMapMode = NormalElem->GetMappingMode();//�}�b�s���O���[�h
	
	FbxLayerElement::EReferenceMode NormalRefMode = NormalElem->GetReferenceMode();//�擾���@

	if (NormalMapMode == FbxLayerElement::eByPolygonVertex)
	{
		if (NormalRefMode == FbxLayerElement::eDirect)
		{
			for (int i = 0; i < NormalNum; ++i) {	// ���ڎ擾
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
			for (int i = 0; i < NormalNum; ++i) {	// ���ڎ擾
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
	FbxLayerElementUV* UVElem = pLayer->GetUVs();				//UV�擾

	int UVCount = UVElem->GetDirectArray().GetCount();			//UV���W�̐�
	int UVindexCount = UVElem->GetIndexArray().GetCount();		//UV�C���f�b�N�X�̐�
	int Size = UVCount > UVindexCount ? UVCount : UVindexCount;	//���̑傫�������T�C�Y��
	vector<XMFLOAT2> UV;
	UV.resize(Size);
	vector<int> s;
	
	FbxLayerElement::EMappingMode UVMapMode = UVElem->GetMappingMode();	//�}�b�s���O���[�h
	FbxLayerElement::EReferenceMode RefMode = UVElem->GetReferenceMode();//�擾���@

	if (UVMapMode == FbxLayerElement::eByPolygonVertex)
	{
		FbxArray<FbxVector2> UVArray;
		UVElem->GetDirectArray().CopyTo(UVArray);

		if (RefMode == FbxLayerElement::eDirect)
		{
			for (int j = 0; j < Size; ++j) {		// ���ڎ擾
				UV[j].x = (float)UVArray.GetAt(j)[0];
				UV[j].y = (float)UVArray.GetAt(j)[1];
			}
		}
		else if (RefMode == FbxLayerElement::eIndexToDirect)
		{
			int index;

			for (int j = 0; j <Size; ++j){			// �C���f�b�N�X����擾
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

//���b�V���̎擾
void MeshFactory::CreateVertices(FbxMesh* pMesh, vector<MyVertex>& pVertex)
{
	int PolygonNum = pMesh->GetPolygonCount();			//�|���S����
	int VertexCount = pMesh->GetControlPointsCount();	//���_��
	if (!(PolygonNum > 1))
		return;
	
	vector<XMFLOAT4>	m_pVertex;		//���_���W
	vector<int>			Poly;		//�|���S���C���f�b�N�X
	vector<XMFLOAT3>	Normal;		//�@���x�N�g��
	vector<XMFLOAT2>	UV;			//UV���W

	int LayerNum = pMesh->GetLayerCount();
	const char* Name=pMesh->GetName();
	FbxLayer* Layer = pMesh->GetLayer(0);
	int PolygonVertexNum = pMesh->GetPolygonVertexCount();
	
	Poly.resize(PolygonVertexNum);
	Normal.resize(PolygonVertexNum);

	SetVertex(pMesh, m_pVertex);//���_�擾
	SetIndex(pMesh, Poly);		//�C���f�b�N�X�擾

	//UV�Ɩ@���̓C���f�b�N�X�Ń\�[�g
	SetNormal(Layer, Normal);	//�@���擾
	//�\�[�g
	vector<XMFLOAT3> SortNorm;
	SortNorm.resize(VertexCount);
	for (int i = 0; i < Normal.size(); i++)
		SortNorm[Poly[i]] = Normal[i];

	SetUV(Layer, UV);			//UV���W�擾
	
	vector<XMFLOAT2> SortUV;
	SortUV.resize(VertexCount);
	for (int i = 0; i < UV.size(); i++)//�\�[�g
		SortUV[Poly[i]] = UV[i];

	pVertex.resize(VertexCount);
	
	for (int i = 0; i < VertexCount; i++)//�f�[�^�i�[
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
	int MaterialNum = pMesh->GetNode()->GetMaterialCount();						//�}�e���A����
	
	if (MaterialNum < 0)
		return;

	int PolygonNum;																//�|���S����
	vector<MyVertex> pVertex;	//���_�z��
	FbxLayerElementArrayTemplate<int>* MaterialIndex;

	CreateVertices(pMesh, pVertex);											//���_�z��͎Q�Ƃ��邽��1�������

	int* Vertics = pMesh->GetPolygonVertices();									//�|���S���̒��_�C���f�b�N�X�z��
	int VertexEnd = Vertices.size();											//�擾���钸�_�̏��߂̃C���f�b�N�X��
	vector<vector<int>> PolygonIndex;
	PolygonIndex.resize(MaterialNum);
	Vertices.resize(Vertices.size() + MaterialNum);

	if (MaterialNum > 1)	//�}�e���A�����Ŏ擾���@�ύX
	{
		pMesh->GetMaterialIndices(&MaterialIndex);//�C���f�b�N�X�ŕ�����
		PolygonNum = MaterialIndex->GetCount();
		vector<int> Index;

		for (int i = 0; i < PolygonNum; i++)
		{
			int PolygonVertexNum = pMesh->GetPolygonSize(i);

			for (int j = 0; j < PolygonVertexNum; j++)
				PolygonIndex[MaterialIndex->GetAt(i)].push_back(pMesh->GetPolygonVertex(i, j));
		}
	}
	else   //�}�e���A����1�ȉ�
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

	for (int i = VertexEnd; i < MaterialNum+VertexEnd; i++)//���i�[
	{
		Vertices[i] = MyMesh(pVertex, PolygonIndex[i]);
	}
}

//�����o�[�g�}�e���A��
void SetLambertInfo(FbxSurfaceLambert* Lambert, MyLambert* Material)
{
	//�A���r�G���g�J���[
	Material->Ambient.x = Lambert->Ambient.Get()[0];
	Material->Ambient.y = Lambert->Ambient.Get()[1];
	Material->Ambient.z = Lambert->Ambient.Get()[2];
	Material->Ambient.w = 1.0;

	//�f�B�t���[�Y
	Material->Diffuse.x = Lambert->Diffuse.Get()[0];
	Material->Diffuse.y = Lambert->Diffuse.Get()[1];
	Material->Diffuse.z = Lambert->Diffuse.Get()[2];

	//����
	Material->Alpha = 1.0f;
}

//phong�}�e���A��
void SetPhongInfo(FbxSurfacePhong* Phong, MyPhong* Material)
{
	//�����o�[�g�Ɠ��������擾
	SetLambertInfo(Phong, Material);

	//�X�y�L����
	Material->Specular.x = Phong->Specular.Get()[0];
	Material->Specular.y = Phong->Specular.Get()[1];
	Material->Specular.z = Phong->Specular.Get()[2];
	//����
	Material->Shiness = Phong->Shininess.Get();
	//���ˌ�
	Material->Refrect = Phong->ReflectionFactor.Get();
}

//�}�e���A���쐬
void MeshFactory::CreateMaterial(FbxMesh* pMesh, MyLambert& MaterialData, vector<ComPtr<ID3D11ShaderResourceView>>& pTex,int MeshNum)
{
	FbxNode* Node = pMesh->GetNode();			//���b�V���̃m�[�h�擾

	if (Node == NULL)
	{
		return;
	}

	int MaterialNum = Node->GetMaterialCount();	//�}�e���A����
	if (MaterialNum == 0)
	{
		return;
	}
	//�}�e���A���i�[
	MyLambert	DummyMaterial;

	FbxSurfaceMaterial* Material = Node->GetMaterial(MeshNum);
	
	if (Material == NULL)
	{
		return;
	}

	if (Material->GetClassId().Is(FbxSurfaceLambert::ClassId))		//�����o�[�g�}�e���A��
	{
		FbxSurfaceLambert* Lambert = (FbxSurfaceLambert*)Material;
		SetLambertInfo(Lambert, &MaterialData);
	}
	else if (Material->GetClassId().Is(FbxSurfacePhong::ClassId))	//�t�H���O�}�e���A��
	{
		FbxSurfacePhong* Phong = (FbxSurfacePhong*)Material;
	}
	else
	{
		SetLambertInfo(NULL, &MaterialData);
	}

	FbxProperty DiffuseProperty = Material->FindProperty(FbxSurfaceMaterial::sDiffuse);//�}�e���A���̃v���p�e�B
	int layerNum = DiffuseProperty.GetSrcObjectCount(FbxCriteria::ObjectType(FbxLayeredTexture::ClassId));	//���C����

	//�e�N�X�`���擾
	if (layerNum > 0)
	{
		//���C���[�h�e�N�X�`��
	}
	else
	{
		//�e�N�X�`�����
		int TexNum = DiffuseProperty.GetSrcObjectCount(FbxCriteria::ObjectType(FbxTexture::ClassId));
		if (TexNum == 0)
			return;

		FbxFileTexture* FileTex = FbxCast<FbxFileTexture>(DiffuseProperty.GetSrcObject(FbxCriteria::ObjectType(FbxTexture::ClassId),0));
		if (FileTex == NULL)
			return;
		
		FbxString Path = FbxPathUtils::GetFileName(FileTex->GetFileName());//�t�@�C���p�X
		const char* TexPath = Path;

		char RelativePath[255] = "../ShaderPractice/Resource/";	
		strcat_s(RelativePath, 255, TexPath);//���΃p�X�֕ϊ�

		
		auto It = m_pTexture->Textures.find(TexPath);

		if (It == m_pTexture->Textures.end())
		{
			//�p�X����e�N�X�`�����擾
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
			pTex.push_back(It->second);		//���ɓ������O�̃e�N�X�`��������Ȃ�ǉ����Ȃ��ŎQ��
		}
	}
}

//�A�j���[�V�����擾
void MeshFactory::CreateAnimInfo(FbxMesh* pMesh,vector<RefBone>& pBone, vector<MySkinAnimation>& pAnim, shared_ptr<MySkelton> pSkelton)
{
	FbxNode* pNode = pMesh->GetNode();		//�m�[�h
	FbxScene* AnimScene = pMesh->GetScene();//�V�[��

	FbxTime::EMode TimeMode;				//�^�C�����[�h�擾
	TimeMode = AnimScene->GetGlobalSettings().GetTimeMode();

	FbxTime Period;							//1�t���[���̎���
	Period.SetTime(0, 0, 0, 1, 0, TimeMode);

	FbxArray<FbxString*> NameArray;			//�A�j���[�V������
	AnimScene->FillAnimStackNameArray(NameArray);
	int AnimNum = NameArray.GetCount();		//�A�j���[�V������
	pAnim.resize(AnimNum);

	FbxTakeInfo* TakeInfo = new FbxTakeInfo;//�e�C�N�̏��
	FbxTime Start;							//�A�j���[�V�����X�^�[�g����
	FbxTime Stop;							//�I������
	int startFrame;							//�A�j���[�V�����X�^�[�g�t���[��
	int stopFrame;							//�I���t���[��
	FbxMatrix MatrixData;					//�p��

	for (int i = 0; i < AnimNum; i++)
	{
		TakeInfo = AnimScene->GetTakeInfo(*(NameArray[i]));
		if (TakeInfo)
		{
			//continue;
		}

		//1�e�C�N�̊J�n�A�I�����Ԏ擾
		Start = TakeInfo->mLocalTimeSpan.GetStart();
		Stop  = TakeInfo->mLocalTimeSpan.GetStop();
		pAnim[i].Start = (int)(Start.Get() / Period.Get());
		pAnim[i].Stop = (int)(Stop.Get() / Period.Get());

		pAnim[i].m_AnimMatrix.resize(pAnim[i].Stop - pAnim[i].Start);

		//�{�[���̏��擾
		CreateBoneInfo(pMesh, pBone,pAnim[i],pSkelton, pAnim[i].Start, pAnim[i].Stop);

		////�A�j���[�V�����R���g���[���ɃZ�b�g
		string Name = NameArray[i]->Buffer();
		pAnim[i].Name = Name;
	}

	TakeInfo = NULL;
	NameArray = NULL;
}

void SetVertexRefBone(FbxCluster* pCluster,vector<RefBone>& pBone, int ClusterNum)
{
	RefBone Bone;
	int CPCount = pCluster->GetControlPointIndicesCount();//�N���X�^�̑Ή����_��

	if (CPCount == 0)
		return;

	int* Vertices = pCluster->GetControlPointIndices();//�N���X�^�̑Ή����_
	double* Weights =pCluster->GetControlPointWeights();//�Ή����_�̃E�G�C�g

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
	FbxAMatrix  InitMat;	//�����p��						
	XMMATRIX	CastMat;	//�ϊ��p�p��
	
	InitMat=pCluster->GetTransformMatrix(InitMat).Inverse();
	CastMatrix(&pBone.m_InitMat, InitMat);
}

void SetAnim(FbxCluster* pCluster, MySkinAnimation& pAnim, int ClusterNum, int StartFrame, int StopFrame)
{
	int CPCount = pCluster->GetControlPointIndicesCount();//�N���X�^�̑Ή����_��

	if (CPCount == 0)
		return;

	FbxAMatrix  InitMat;	//�����p��						
	FbxMatrix	FrameMat;	//�t���[���p��
	XMMATRIX	CastMat;	//�ϊ��p�p��

	pAnim.m_AnimBoneMatrix[ClusterNum].reserve(StopFrame - StartFrame);
	FbxNode* pNode = pCluster->GetLink();//�A�j���[�V�����̃m�[�h
	FbxTime Time;

	for (int i = 0; i < StopFrame-StartFrame; i++)//�e�t���[���̎p���擾
	{
		Time.SetSecondDouble(i);
		FrameMat = pNode->EvaluateGlobalTransform(StopFrame).Inverse();
		CastMatrix(&CastMat, FrameMat);
		pAnim.m_AnimBoneMatrix[ClusterNum].push_back(CastMat);
	}
}

//�{�[���̏��擾
void MeshFactory::CreateBoneInfo(FbxMesh* pMesh, vector<RefBone>& pRefBone,MySkinAnimation& pAnim, shared_ptr<MySkelton> pSkelton, int StartFrame,int StopFrame)
{
	int SkinCount= pMesh->GetDeformerCount(FbxDeformer::eSkin);	//�X�L����
	FbxSkin* pSkin;												//�X�L��
	int ClusterNum = 0;											//�N���X�^��
	FbxCluster* pCluster;										//�N���X�^
	
	for (int i = 0; i < SkinCount; i++)
	{
		pSkin = (FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);//�X�L�����擾
		ClusterNum += pSkin->GetClusterCount();
		pSkelton->m_Bone.resize(ClusterNum);
		pAnim.m_AnimBoneMatrix.resize(ClusterNum);

		for (int j = 0; j < ClusterNum; j++)		//�{�[���̂ƃA�j���[�V�������擾
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

//�X�v���C�g�쐬
bool MeshFactory::CreateSprite(const char* FileName, shared_ptr<Sprite>& pSprite, RECT Rect, RECT Size)
{
	MyMesh Mesh;
	//�X�v���C�g�̒��_��`
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

	//�X�v���C�g�̒��_�C���f�b�N�X��`
	int PolygonCount = 6;
	vector<int> Index;
	Index.resize(6);
	Index[0] = 0;
	Index[1] = 1;
	Index[2] = 3;
	Index[3] = 1;
	Index[4] = 2;
	Index[5] = 3;


	//�e�N�X�`��
	ID3D11ShaderResourceView* Tex;
	char RelativePath[255] = "../ShaderPractice/Resource/";
	strcat_s(RelativePath, 255, FileName);//���΃p�X�֕ϊ�

	D3DX11CreateShaderResourceViewFromFile(m_pDevice->GetDevice(), RelativePath, NULL, NULL, &Tex, NULL);

	Mesh = MyMesh(Vertex, Index);
	pSprite = make_shared<Sprite>(Mesh, Tex);
	pSprite->m_Material.Alpha = 1.0;

	//�o�b�t�@�쐬
	pSprite->CreateVertBuffer(m_pDevice->GetDevice());
	pSprite->CreateIndexBuffer(m_pDevice->GetDevice());
	pSprite->CreateMaterialBuffer(m_pDevice->GetDevice());

	//�e�N�X�`���T���v��
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	m_pDevice->GetDevice()->CreateSamplerState(&SamDesc, &pSprite->m_pSampleLiner);

	return true;
}

//���
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