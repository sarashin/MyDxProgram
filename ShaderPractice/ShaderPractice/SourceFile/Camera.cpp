#include"stdafx.h"
#include "Camera.h"

//�R���X�g���N�^
Camera::Camera()
{
	XMStoreFloat4x4(&m_ViewMat, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMat, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMat, XMMatrixIdentity());
	XMStoreFloat4x4(&m_LocalMat, XMMatrixIdentity());
}

//�f�X�g���N�^
Camera::~Camera()
{
}

//�J�����̉�p�s��ݒ�
void Camera::SetProjMat(float Rad, float Aspect, float ZNear, float ZFar)
{
	/*XMVECTOR InitVec=XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX View = XMMatrixIdentity();
	View *= XMMatrixRotationQuaternion(InitVec);
	View *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&m_ViewMat, View);
*/
	XMMATRIX Proj = XMMatrixIdentity();
	Proj = XMMatrixPerspectiveFovLH(Rad, Aspect, ZNear, ZFar);
	XMStoreFloat4x4(&m_ProjMat, Proj);
}

//�J�����̃r���[�s��쐬
void Camera::SetViewMat(XMFLOAT3 Eye,XMFLOAT3 At,XMFLOAT3 Up)
{
	XMMATRIX ViewMat=XMMatrixLookAtLH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up));
	XMStoreFloat4x4(&m_ViewMat, ViewMat);
}

//���[�J���̃J�����ʒu�̈ړ�
void Camera::TranslateL(XMVECTOR Trans)
{
	XMMATRIX Pos;
	Pos = XMLoadFloat4x4(&m_LocalMat)*XMMatrixTranslationFromVector(Trans);
	XMStoreFloat4x4(&m_LocalMat, Pos);
}

//���[�J���̃J�����p���̕ύX
void Camera::SetTranslateL(XMVECTOR Trans)
{
	XMMATRIX Pos;
	Pos = XMMatrixTranslationFromVector(Trans);
	XMStoreFloat4x4(&m_LocalMat, Pos);
}

//���[�J���̃J�����̉�]
void Camera::RotationL(XMVECTOR Quatanion)
{
	XMMATRIX View = XMMatrixIdentity();
	XMMATRIX Quad = XMMatrixRotationQuaternion(Quatanion);
	Quad = XMMatrixInverse(Quad.r, Quad);
	View = XMLoadFloat4x4(&m_LocalMat) * Quad;
	XMStoreFloat4x4(&m_LocalMat, View);
}

void Camera::RotationL(XMFLOAT3 Rotate)
{
	XMVECTOR Quad=XMQuaternionRotationRollPitchYaw(Rotate.x,Rotate.y,Rotate.z);
	RotationL(Quad);
}


//���[���h�̃J�����ʒu�̈ړ�
void Camera::TranslateW(XMVECTOR Trans)
{
	XMMATRIX World = XMMatrixIdentity();
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos,Trans);
	World = XMLoadFloat4x4(&m_WorldMat)*XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
	XMStoreFloat4x4(&m_WorldMat,World);
}
//���[���h�̃J�����ʒu�̕ύX
void Camera::SetTranslateW(XMVECTOR Trans)
{ 
	XMMATRIX World=XMLoadFloat4x4(&m_WorldMat);
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos, Trans);
	
	World._41 = Pos.x;
	World._42 = Pos.y;
	World._43 = Pos.z;
	XMMATRIX InvMat=XMLoadFloat4x4(&m_WorldMat);
	InvMat = XMMatrixInverse(World.r, World);
	XMStoreFloat4x4(&m_WorldMat, InvMat);
}

//���[���h���W�ϊ�����View�s��擾
XMMATRIX Camera::GetViewMatW()
{
	XMMATRIX CameraW =XMLoadFloat4x4(&m_WorldMat) * XMLoadFloat4x4(&m_LocalMat) * XMLoadFloat4x4(&m_ViewMat);
	
	return CameraW;
}

//�J�����̃��[�J�����W�擾
XMMATRIX Camera::GetViewMatL()
{
	return XMLoadFloat4x4(&m_ViewMat);
}

//�J�������W�擾
XMMATRIX Camera::GetWorldMat()
{
	XMMATRIX WorldMat = XMLoadFloat4x4(&m_WorldMat);
	XMMATRIX LocalMat = XMLoadFloat4x4(&m_LocalMat);
	return WorldMat*LocalMat;
}

//�v���W�F�N�V�����s��擾
XMMATRIX Camera::GetProjMat()
{
	return XMLoadFloat4x4(&m_ProjMat);
}

//�J�����̍��W�擾
CXMVECTOR Camera::GetPos()
{
	XMMATRIX View = XMMatrixIdentity();
	View = XMLoadFloat4x4(&m_ViewMat);
	return XMMatrixDeterminant(View);
}

//�J�����̑O���x�N�g��
CXMVECTOR Camera::GetFlont()
{
	XMMATRIX CameraW = XMLoadFloat4x4(&m_WorldMat) * XMLoadFloat4x4(&m_LocalMat) * XMLoadFloat4x4(&m_ViewMat);
	XMVECTOR Flont=XMLoadFloat3(&XMFLOAT3(CameraW._33-CameraW._23,0.0f,CameraW._31 - CameraW._21));

	return XMVector3Normalize(Flont);
}