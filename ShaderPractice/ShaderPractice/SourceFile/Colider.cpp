#include"stdafx.h"
#include "Colider.h"
#include"PhysicsWorld.h"
using namespace physx;

//�R���X�g���N�^
Collider::Collider()
{
	m_pCollider = NULL;
	m_pEnterFunc = NULL;
	m_pExitFunc = NULL;
}

//�R���X�g���N�^
//pActor:�A�N�^�̃|�C���^
Collider::Collider(PxRigidStatic* pActor)
{
	m_pCollider = pActor;
	SetUpFilterGroup(m_pCollider, FilterGroup::eBOX, FilterGroup::eLAND);
	m_pCollider->userData = this;
	m_pEnterFunc = NULL;
	m_pExitFunc = NULL;
}

//�f�X�g���N�^
Collider::~Collider()
{
	Release();
}

//������
void Collider::Init()
{
	Enter.clear();
	Contact.clear();
	Contacted.clear();
}

//�X�V
void Collider::Update()
{
	vector<PhysXActor*>::iterator EnterIt = Enter.begin();

	//�ڐG���Ă�����̂̃R�[���o�b�N�֐������s
	if (Enter.empty() == false && m_pEnterFunc != NULL)
	{
		while (EnterIt != Enter.end())
		{
			m_pEnterFunc(pObject, *EnterIt);
			Enter.erase(EnterIt);

			if (Enter.empty() == true)
				break;

			EnterIt++;
		}
	}

	vector<PhysXActor*>::iterator ExitIt = Contacted.begin();

	//���ꂽ���̂̃R�[���o�b�N�֐������s
	if (Contacted.empty() == false && m_pExitFunc != NULL)
	{
		while (ExitIt != Contacted.end())
		{
			m_pExitFunc(pObject, *ExitIt);
			Contacted.erase(ExitIt);

			if (Contacted.empty() == true)
				break;

			ExitIt++;
		}
	}
}

//�A�N�^�̃Z�b�g
void Collider::SetActor(PxRigidStatic* Collider)
{
	m_pCollider = Collider;
}

//PhysX�̎p�����擾
Transform Collider::GetPxTrans()
{
	if (m_pCollider == NULL)
	{
		return XMMatrixIdentity();
	}

	PxTransform PxTrans;
	PxTrans = m_pCollider->getGlobalPose();
	//�ʒu�@XMFLOAT3�ɃL���X�g
	PxVec3 PxPos = PxTrans.p;
	XMFLOAT3 Pos;
	Pos.x = PxPos.x;
	Pos.y = PxPos.y;
	Pos.z = PxPos.z;
	//��]�@XMVECTOR�ɃL���X�g
	PxQuat PxRotate = PxTrans.q;
	XMVECTOR Quatanion;
	XMFLOAT4 Rotate;
	Rotate.x = PxRotate.x;
	Rotate.y = PxRotate.y;
	Rotate.z = PxRotate.z;
	Rotate.w = PxRotate.w;
	Quatanion = XMLoadFloat4(&Rotate);

	XMMATRIX TransMat = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	XMMATRIX RotateMat = XMMatrixRotationQuaternion(Quatanion);

	return Transform(RotateMat*TransMat);
}

//�A�N�^�[�擾
physx::PxRigidActor* Collider::GetActor()
{
	return m_pCollider;
}

//�p�����Z�b�g
void Collider::SetPxTrans(Transform Trans)
{
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos, Trans.GetPosition());
	PxVec3 PxPos;
	PxPos.x = Pos.x;
	PxPos.y = Pos.y;
	PxPos.z = Pos.z;

	XMFLOAT4 Quat;
	XMStoreFloat4(&Quat, Trans.GetRotationQuatanion());
	PxQuat PxQu;
	PxQu.x = Quat.x;
	PxQu.y = Quat.y;
	PxQu.z = Quat.z;
	PxQu.w = Quat.w;

	m_pCollider->setGlobalPose(PxTransform(PxPos));
}

//�Փ˃t�B���^�[�ݒ�
void Collider::SetUpFilterGroup(physx::PxRigidActor* Actor, physx::PxU32 FilterGroup, physx::PxU32 FilterMask)
{
	PxFilterData FilterData;
	FilterData.word0 = FilterGroup;
	FilterData.word1 = FilterMask;

	const PxU32 ShapeNum = m_pCollider->getNbShapes();

	for (int i = 0; i < ShapeNum; i++)
	{
		PxShape* Shape;
		m_pCollider->getShapes(&Shape, 1, i);
		Shape->setSimulationFilterData(FilterData);
	}
}

//�ڐG���R�[���o�b�N�֐�
void Collider::SetEnterCallBack(FUNC_POINTER EnterCall, void* Cls)
{
	m_pEnterFunc = EnterCall;
	pObject = Cls;
}

//���ꂽ���̃R�[���o�b�N�֐�
void Collider::SetExitCallBack(FUNC_POINTER ExitCall, void* Cls)
{
	m_pExitFunc = ExitCall;
	pObject = Cls;
}

//�ڐG������PhysX�ŌĂ΂�� PhysXActor�͐�L���b�N���|����̂ł��Ƃŏ���
void Collider::OnCollisionEnter(PhysXActor* pTarget)
{
	if (pTarget != NULL)
		Enter.push_back(pTarget);
}

//���ꂽ��PhysX�ŌĂ΂�� PhysXActor�͐�L���b�N���|����̂ł��Ƃŏ���
void Collider::OnCollisionExit(PhysXActor* pTarget)
{
	if (pTarget != NULL)
		Contacted.push_back(pTarget);
}

//���
void Collider::Release()
{
	if (m_pCollider != NULL)
	{
		PhysicsWorld::GetInstance().GetScene()->removeActor(*m_pCollider);
		m_pCollider->release();
		m_pCollider = NULL;
	}
}