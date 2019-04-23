#include"stdafx.h"
#include"Transform.h"



//�R���X�g���N�^
Transform::Transform()
{
	XMStoreFloat4x4(&TransformMatrix, XMMatrixIdentity());
}

Transform::Transform(CXMMATRIX Mat)
{
	XMStoreFloat4x4(&TransformMatrix, Mat);
}

Transform::Transform(XMFLOAT3 Pos)
{
	XMStoreFloat4x4(&TransformMatrix,XMMatrixTranslationFromVector(XMLoadFloat3(&Pos)));
}

//�f�X�g���N�^
Transform::~Transform()
{
}

//�p���̃Z�b�g
void Transform::SetMat(CXMMATRIX Mat)
{
	XMStoreFloat4x4(&TransformMatrix,Mat);
}

//�p���̎擾
XMMATRIX Transform::GetMat()
{
	return XMLoadFloat4x4(&TransformMatrix);
}

//�ʒu�̃Z�b�g
void Transform::SetPosition(XMVECTOR Pos)
{
	XMFLOAT3 Position;
	XMStoreFloat3(&Position, Pos);
	TransformMatrix._41 = Position.x;
	TransformMatrix._42 = Position.y;
	TransformMatrix._43 = Position.z;
}

void Transform::SetPosition(float x, float y, float z)
{
	TransformMatrix._41 = x;
	TransformMatrix._42 = y;
	TransformMatrix._43 = z;
}

//�ʒu�̈ړ�
void Transform::MovePosition(XMVECTOR Vec)
{
	XMFLOAT3 MoveVec;
	XMStoreFloat3(&MoveVec, Vec);
	TransformMatrix._41 += MoveVec.x;
	TransformMatrix._42 += MoveVec.y;
	TransformMatrix._43 += MoveVec.z;
}

void Transform::MovePosition(float x,float y,float z)
{
	TransformMatrix._41 += x;
	TransformMatrix._42 += y;
	TransformMatrix._43 += z;
}

//�ʒu�̎擾
XMVECTOR Transform::GetPosition()
{
	float x, y, z;
	XMFLOAT3 Pos;
	Pos.x = TransformMatrix._41;
	Pos.y = TransformMatrix._42;
	Pos.z = TransformMatrix._43;
	return XMLoadFloat3(&Pos);
}

//��]�̃Z�b�g
void Transform::SetRotation(XMVECTOR Rotation)
{
	XMMATRIX RotationMat,TransMat;
	RotationMat = XMMatrixRotationRollPitchYawFromVector(Rotation);
	TransMat = XMMatrixTranslation(TransformMatrix._41, TransformMatrix._42, TransformMatrix._43);
	XMStoreFloat4x4(&TransformMatrix,RotationMat*TransMat);
}

void Transform::SetRotation(CXMMATRIX RotationMat)
{
	XMMATRIX RotateMat, TransMat;
	XMVECTOR Quat;
	Quat = XMQuaternionRotationMatrix(RotationMat);
	RotateMat = XMMatrixRotationRollPitchYawFromVector(Quat);
	TransMat = XMMatrixTranslation(TransformMatrix._41, TransformMatrix._42, TransformMatrix._43);
	XMStoreFloat4x4(&TransformMatrix, RotateMat*TransMat);
}

//�ʒu�̉�]
void Transform::RotationQuatanion(XMVECTOR Vec)
{
	XMMATRIX RotateMat = XMMatrixRotationQuaternion(Vec);
	XMMATRIX Mat = XMLoadFloat4x4(&TransformMatrix)*RotateMat;
	XMStoreFloat4x4(&TransformMatrix,Mat);
}

//�ʒu�̉�]
void Transform::Rotation(float x,float y,float z)
{
	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(x, y, z);
	XMMATRIX Mat = XMLoadFloat4x4(&TransformMatrix)*RotateMat;
	XMStoreFloat4x4(&TransformMatrix, Mat);
}


//��]�p�x�̎擾
XMVECTOR Transform::GetRotationQuatanion()
{
	XMMATRIX Mat = XMLoadFloat4x4(&TransformMatrix);
	return XMQuaternionRotationMatrix(Mat);
}

//�X�P�[���̊g�k
void Transform::AddScale(float Scale)
{
	TransformMatrix._11 *= Scale;
	TransformMatrix._22 *= Scale;
	TransformMatrix._33 *= Scale;
}

//�X�P�[���̎擾
XMVECTOR Transform::GetScale()
{
	XMVECTOR XScale,YScale,ZScale;
	FLOAT	X, Y, Z;
	XScale = XMVector3Length(XMLoadFloat3(&XMFLOAT3(TransformMatrix._11, TransformMatrix._12, TransformMatrix._13)));
	YScale = XMVector3Length(XMLoadFloat3(&XMFLOAT3(TransformMatrix._21, TransformMatrix._22, TransformMatrix._23)));
	ZScale = XMVector3Length(XMLoadFloat3(&XMFLOAT3(TransformMatrix._31, TransformMatrix._32, TransformMatrix._33)));

	XMStoreFloat(&X, XScale);
	XMStoreFloat(&Y, YScale);
	XMStoreFloat(&Z, ZScale);

	return XMLoadFloat3(&XMFLOAT3(X, Y, Z));
}

XMFLOAT3 Transform::GetFlont()
{
	XMFLOAT3 Flont = XMFLOAT3(TransformMatrix._33 - TransformMatrix._23, 0.0f, TransformMatrix._31 - TransformMatrix._21);
	return Flont;
}

XMFLOAT3 Transform::GetRight()
{
	return XMFLOAT3(TransformMatrix._31 - TransformMatrix._21, 0.0,TransformMatrix._33-TransformMatrix._23);
}