#pragma once
#include"Transform.h"

//�A�N�^�̃x�[�X�N���X
class ActorObject
{
protected:
	Transform m_InitMat;
public:
	//PhysX��̍��W�擾
	virtual Transform	GetPxTrans() = 0;
	//���
	virtual void		Release() = 0;


};