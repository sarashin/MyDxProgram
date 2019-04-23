#pragma once
#include"Window.h"

//�A�v���P�[�V�����N���X
class AppBase
{
private:
protected:
	//�C���X�^���X�n���h��
	HINSTANCE m_hInstance;
	//�E�C���h�E�N���X
	WNDCLASSEX m_WndClass;
	//�E�C���h�E�z��
	vector<shared_ptr<Window>> m_pWndAry;	
	//�E�C���h�E�N���X�ɏ����Z�b�g
	virtual HRESULT RegistWndClass();		
	//�E�C���h�E�N���X�쐬
	virtual HRESULT CreateMainWindow();		

	//������
	virtual bool	Init()=0;		
	//�X�V
	virtual void	Update();				
	//���
	virtual void	Release()=0;			
public:
	//�R���X�g���N�^
	AppBase();			
	//�f�X�g���N�^
	virtual ~AppBase(); 

	//�A�v���P�[�V���������s
	HRESULT Run(HINSTANCE hInst);			
	//�O���[�o���E�C���h�E�v���V�[�W��
	static LRESULT CALLBACK GrobalWndProc( HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
	//�C���X�^���X�n���h���̎擾
	HINSTANCE GetHInstance();				
	//�E�C���h�E�N���X�擾
	void GetWindowClass(unique_ptr<WNDCLASSEX> pWndClass);
	//�E�C���h�E�擾
	vector<shared_ptr<Window>>* GetWndPtr();
};

class Application:public AppBase
{
public:
	//�A�v���P�[�V����
	unique_ptr<DxBase> m_pDxBase;			

	//�R���X�g���N�^
	Application();							
	//�f�X�g���N�^
	~Application();							
	//������
	bool	Init();					
	//�X�V
	void	Update();			
	//�E�C���h�E�N���X�ɏ����Z�b�g
	HRESULT RegistWndClass()override;		
	//�E�C���h�E�N���X�쐬
	HRESULT CreateMainWindow()override;		
	//���
	void	Release();					
};
