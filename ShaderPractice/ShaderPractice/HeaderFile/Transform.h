#pragma once
#include <xnamath.h>

class Transform
{
public:
	//�p��
	XMFLOAT4X4 TransformMatrix;
	
	//�R���X�g���N�^
	Transform();

	//�R���X�g���N�^
	//Pos:�������W
	Transform(XMFLOAT3 Pos);

	//�R���X�g���N�^
	//Mat:�����p��
	Transform(CXMMATRIX Mat);

	//�f�X�g���N�^
	~Transform();

	void SetMat(CXMMATRIX Matrix);		//�p���̃Z�b�g
	XMMATRIX GetMat();					//�p���擾
	void SetPosition(XMVECTOR Pos);		//�ʒu�̃Z�b�g
	void SetPosition(float x, float y, float z);	//�ʒu�̃Z�b�g
	void MovePosition(XMVECTOR Pos);	//�ʒu�𓮂���
	void MovePosition(float x,float y,float z);	//�ʒu�𓮂���
	XMVECTOR GetPosition();				//�ʒu�̎擾
	void SetRotation(XMVECTOR Rotation);//��]�̃Z�b�g
	void SetRotation(CXMMATRIX RotationMat);
	void RotationQuatanion(XMVECTOR Rot);//�C�ӎ���]
	void Rotation(float x, float y, float z);
	XMVECTOR GetRotationQuatanion();	//��]�p�x�擾
	void AddScale(float Scale);			//�X�P�[���̊g�k
	XMVECTOR GetScale();				//�X�P�[���̎擾

	//�O���x�N�g���擾
	XMFLOAT3 GetFlont();
	//�E�x�N�g���擾
	XMFLOAT3 GetRight();
};

