#pragma once
#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))

class Camera
{
	//�^�[�Q�b�g�̃��[���h���W
	XMFLOAT4X4 m_WorldMat;
	//���[�J�����W
	XMFLOAT4X4 m_LocalMat;
	//�r���[�s��
	XMFLOAT4X4 m_ViewMat;
	//�v���W�F�N�V�����s��
	XMFLOAT4X4 m_ProjMat;
public:
	
	//�R���X�g���N�^
	Camera();

	//�f�X�g���N�^
	~Camera();

	//�v���W�F�N�V�����s��ݒ�
	//Rad    : ��p
	//Aspect : �A�X�y�N�g��
	//ZNear  : �J�����̍ŒZ�\������
	//ZFar   : �J�����̍Œ��\������
	void SetProjMat(float Rad,float Aspect,float ZNear,float ZFar);

	//�r���[�s��ݒ�
	//Eye: ���_���W
	//At : �����x�N�g��
	//Up : �A�b�v�x�N�g��
	void SetViewMat(XMFLOAT3 Eye,XMFLOAT3 At,XMFLOAT3 Up);
	//�J�����̃��[�J���ł̈ړ�
	//Trans : �ړ�����
	void TranslateL(XMVECTOR Trans);
	//�J�����̃��[�J�����W�ݒ�
	//Trans : �ʒu���W
	void SetTranslateL(XMVECTOR Trans);
	//�J�����̃��[�J����]
	//Quatanion : ��]�̃N�I�[�^�j�I��
	void RotationL(XMVECTOR Quatanion);
	//�J�����̃��[�J����]
	//Rotate:�J�����̃��[���A�s�b�`�A���[��]
	void RotationL(XMFLOAT3 Rotate);
	//�J�����̃��[���h���W�ړ�
	//Trans:�ړ�����
	void TranslateW(XMVECTOR Trans);
	//�J�����̃��[���h���W�ݒ�
	//Trans:�ړ����W
	void SetTranslateW(XMVECTOR Trans);
	void RotationW(XMVECTOR Quatanion);
	void SetRotationW(XMVECTOR Quatanion);
	//�ʒu�̎擾
	CXMVECTOR GetPos();
	//�O���x�N�g���擾
	CXMVECTOR GetFlont();

	//���[���h�s��擾
	XMMATRIX GetWorldMat();
	//���[�J���r���[�s��擾
	XMMATRIX GetViewMatL();
	//���[���h�r���[�s��擾
	XMMATRIX GetViewMatW();
	//�v���W�F�N�V�����s��擾
	XMMATRIX GetProjMat();
};

