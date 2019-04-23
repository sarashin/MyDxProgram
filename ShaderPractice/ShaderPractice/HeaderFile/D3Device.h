#pragma once
#include"Mesh.h"
#include"Camera.h"
#include"MeshObject.h"
#include"SpriteObject.h"

#define SAFE_RELEASE(x) if(x){x->Release();x=NULL;}

class D3DRenderer
{
private:
	//DirectX�̕ϐ�
	//�f�o�C�X
	ComPtr<ID3D11Device>			m_pDevice;
	//�f�o�C�X�R���e�L�X�g
	ComPtr<ID3D11DeviceContext>		m_pDevContext;
	//�X���b�v�`�F�C��
	ComPtr<IDXGISwapChain>			m_pSwapChain;
	//�����_�[�^�[�Q�b�g
	ComPtr<ID3D11RenderTargetView>	m_pRenderTrgView;
	//�[�x�o�b�t�@
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;
	//�[�x�o�b�t�@�p�e�N�X�`��
	ComPtr<ID3D11Texture2D>			m_pDepthStencilTex;
	//�u�����h�X�e�[�g
	ComPtr<ID3D11BlendState>		m_pBlendState;
	
protected:
public:
	//�f�t�H���g�V�F�[�_
	ComPtr<ID3D11VertexShader>		m_pDefVShader;
	ComPtr<ID3D11PixelShader>		m_pDefPShader;

	//�R���X�g���N�^
	D3DRenderer();
	//�f�X�g���N�^
	~D3DRenderer();

	//�E�C���h�E�T�C�Y
	CRect rect;

	//�f�o�C�X�擾
	ID3D11Device*			GetDevice();
	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext*	GetDevContext();
	//�����_�[�^�[�Q�b�g�擾
	ID3D11RenderTargetView** GetRenderTarget();

	//�V�F�[�_�쐬
	//Pass : �t�@�C���p�X
	HRESULT CreateVShader(LPCSTR Pass);
	HRESULT CreatePShader(LPCSTR Pass);

	//D3D11�쐬
	//hWnd:�E�C���h�E�n���h��
	HRESULT CreateDevice(HWND hWnd);
	
	//�`��J�n�A�I��
	void RenderStart();
	void RenderEnd();
	//�[�x�o�b�t�@����
	void InvalidDepthStencil();
	//���
	void Release();
};

//�V�F�[�_�֓n�����
struct ShaderGlobal
{
	XMMATRIX WorldMat;
	XMMATRIX ViewMat;
	XMMATRIX ProjMat;
};