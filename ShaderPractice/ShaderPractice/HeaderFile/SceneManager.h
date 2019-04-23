#pragma once
//#include <d3d11.h>
//#include <d3dx11.h>
#include"D3Device.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#include"SceneBase.h"
#include"SceneFactory.h"

//�V�[���̍X�V�ƃt�F�[�h�C���A�E�g���s���N���X
class SceneManager:public Singleton<SceneManager>
{
private:
	friend class Singleton<SceneManager>;
	//�V�[���t�@�N�g��
	shared_ptr<SceneFactory>	m_pFactory;	
	//�V�[��
	unique_ptr<SceneBase>		m_pScene;	
	//���[�h����UI�\���p�J����
	Camera						m_Camera;	
	//�����_���[
	shared_ptr<D3DRenderer>		m_pRenderer;
	//�V�[������̃��b�Z�[�W
	DWORD						SceneMsg;	
	//�X�e�[�W
	StageData					Stage;

	//�t�F�[�h�C���A�A�E�g�̃X���b�h
	std::thread Fade;

	SceneManager();
	SceneManager(const SceneManager&) {};

public:
	//���[�h����
	bool IsLoading;	
	//�t�F�[�h�C�����Ă��邩
	bool IsFading;	
	//�V�[����ς��邩
	bool IsChange;	

	//���[�h��UI
	unique_ptr<SpriteObject>	m_pFadeSprite;				

	//�f�X�g���N�^
	~SceneManager();
	//������
	//pFactory:�V�[���̃t�@�N�g��
	//hWnd:�E�C���h�E�n���h��
	//hInst:�C���X�^���X�n���h��
	void Init(shared_ptr<SceneFactory> pFactory,HWND hWnd,HINSTANCE hInst);	

	//���V�[���̍X�V
	void SceneUpdate();				

	//�t�F�[�hUI�̍쐬
	void CreateFadeSprite();			

	//���V�[���̕`��
	void Render();							

	//�t�F�[�h�C��
	void FadeIn();						

	//�t�F�[�h�A�E�g
	void FadeOut();											
	
	//�V�[���̃��[�h
	//SceneName:�V�[����
	void Load(DWORD SceneName);			

	//���
	void Release();											
};