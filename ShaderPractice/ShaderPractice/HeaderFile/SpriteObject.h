#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "D3Device.h"

class SpriteObject :public GameObject
{
	//�X�v���C�g�̃|�C���^
	shared_ptr<Sprite>	m_pSprite;
public:
	//�\�����邩
	bool				IsAppear;
	//���W
	Transform			m_WorldMat;

	//�V�F�[�_�֓n���o�b�t�@
	ComPtr<ID3D11Buffer>			m_pMatrixBuffer;

	//�R���X�g���N�^
	SpriteObject();
	//�f�X�g���N�^
	~SpriteObject();

	//���b�V���̐ݒ�
	void SetMeshObject(shared_ptr<Sprite>& pSprite);
	//���b�V���̎擾
	Sprite* GetMeshObject();

	//�X�v���C�g�̍쐬
	void Create(const char* TexPath, ComPtr<ID3D11Device> pDevice);
	//�X�V
	void Update();
	//�`��
	void Draw(Camera Cam,ComPtr<ID3D11DeviceContext> pDevContext, ID3D11VertexShader* VShader, ID3D11PixelShader* PShader);
	//�I�u�W�F�N�g�̃R�s�[
	void CopyObject(SpriteObject** ppObject);
	//���
	void Release();
};

