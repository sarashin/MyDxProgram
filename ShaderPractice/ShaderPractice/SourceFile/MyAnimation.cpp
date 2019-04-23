#include"stdafx.h"
#include"MyAnimation.h"

MySkinAnimation::MySkinAnimation()
{
	
}

MySkinAnimation::~MySkinAnimation()
{
	m_AnimBoneMatrix.clear();
	m_AnimMatrix.clear();
}

MyAnimationContainer::MyAnimationContainer()
{
	m_pMesh = make_shared<MyMeshData>();
}

MyAnimationContainer::MyAnimationContainer(vector<MySkinAnimation> Anim)
{
	m_pAnimation = make_shared<vector<MySkinAnimation>>();
	*m_pAnimation = Anim;
}

//アニメーション更新
void MyAnimationContainer::UpdateAnimation(int AnimIndex, float Frame, MySkelton& pSkelton)
{
	int Size = pSkelton.GetBoneNum();
	MySkinAnimation Anim = m_pAnimation->at(AnimIndex);

	for (int i = 0; i < Size; i++)
	{
		if (Anim.m_AnimBoneMatrix[i].size() == 0)
			continue;

		pSkelton.m_Bone[i].m_Matrix = Anim.m_AnimBoneMatrix[i][Frame];
	}
}
