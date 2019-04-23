#pragma once
#include"Transform.h"
#include"ActorObject.h"
using namespace physx;

//PhysX�̃t�B���^�Ɏg���\����
struct FilterGroup
{
	enum Filter
	{
		eSPHERE = (1 << 0),
		eBOX = (1 << 1),
		eCAPSUEL = (1 << 2),
		eCONVEX = (1 << 3),
		eLAND = (1 << 4),
	};
};

//�`���enum
enum SHAPE
{
	BOX=0,
	SPHERE,
	CAPSUEL,
	CONVEX,
	TRI,
};

//PhysX�̃A�N�^
class PhysXActor
{
protected:
	//�t�B���^
	//pActor:�A�N�^�̃|�C���^
	//FilterGroup:������t�B���^
	//FilterMask:�ǂ���t�B���^���邩
	virtual void SetUpFilterGroup(physx::PxRigidActor* pActor, physx::PxU32 FilterGroup, physx::PxU32 FilterMask)=0;
	
public:
	//������
	virtual void		Init() = 0;
	//�X�V
	virtual void		Update()=0;
	//PhysX��̍��W�擾
	virtual Transform	GetPxTrans()=0;
	//�A�N�^�̎擾
	virtual PxRigidActor* GetActor()=0;
	//���
	virtual void		Release()=0;
};

