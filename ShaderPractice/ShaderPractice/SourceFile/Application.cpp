#include"stdafx.h"
#include"Application.h"
#include"GrobalTimer.h"

#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))

//���s�A�v���P�[�V����
AppBase* pApp;

//�O���[�o���^�C�}�[
GrobalTimer& s_GrobalTimer = GrobalTimer::GetInstance();

//�R���X�g���N�^
AppBase::AppBase() 
{
	pApp = this;
}

//�f�X�g���N�^
AppBase::~AppBase()
{

}

//�E�C���h�E�̃O���[�o���v���V�[�W��
LRESULT CALLBACK AppBase::GrobalWndProc(HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	if (pApp!=NULL)
	{
		vector<shared_ptr<Window>>* pWnd = pApp->GetWndPtr();
		int Size = pWnd->size();
		if (Size > 2)
			Size = 2;

		//���[�J���̃E�C���h�E�v���V�[�W���֏������ϑ�
		for (int i = 0; i < Size; i++)
		{
			Window* WndBase = (*pWnd)[i].get();
			if (WndBase)
				return WndBase->LocalWndProc(hWnd, Msg, wParam, lParam);
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}


//�E�C���h�E�̒�`
HRESULT AppBase::RegistWndClass()
{
	m_WndClass= { sizeof(WNDCLASSEX),
		CS_CLASSDC,
		GrobalWndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		"Window",
		NULL };

	if (RegisterClassEx(&m_WndClass) > 0)
		return S_OK;
	else
		return HRESULT(-1);
}

//���C���E�C���h�E�쐬
HRESULT AppBase::CreateMainWindow()
{
	shared_ptr<Window> pWnd = make_shared<Window>();
	pWnd->Create(m_WndClass.hInstance, "MainWindow",NULL);
	m_pWndAry.push_back(pWnd);

	return S_OK;
}

//�X�V����
void AppBase::Update()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//�A�v���P�[�V�����̓���
HRESULT AppBase::Run(HINSTANCE hInst)
{
	HRESULT hr = S_OK;
	m_hInstance = hInst;

	//�E�C���h�E�̒��^�쐬
	if(FAILED(this->RegistWndClass()))
	{
		hr = HRESULT(-1);
		return hr;
	}

	//���C���E�C���h�E�쐬
	if(FAILED(this->CreateMainWindow()))
	{
		hr = HRESULT(-1);
		return hr;
	}

	//������
	if (!this->Init())
	{
		hr = HRESULT(-1);
		return hr;
	}

	//�X�V
	this->Update();

	//���
	this->Release();
}

//�C���X�^���X�n���h���擾
HINSTANCE AppBase::GetHInstance()
{
	return m_hInstance;
}

//�E�C���h�E�N���X�̎擾
void AppBase::GetWindowClass(unique_ptr<WNDCLASSEX> WndClass)
{
	*WndClass = m_WndClass;
}

//�E�C���h�E�̃|�C���^�擾
vector<shared_ptr<Window>>* AppBase::GetWndPtr()
{
	return &m_pWndAry;
}

//�R���X�g���N�^
Application::Application()
{
	pApp = this;
}

//�f�X�g���N�^
Application::~Application()
{

}

//�E�C���h�E�̒�`
HRESULT Application::RegistWndClass()
{
	m_WndClass = { sizeof(WNDCLASSEX),
		CS_CLASSDC,
		GrobalWndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		"PhysX Program",
		NULL };

	if (RegisterClassEx(&m_WndClass) > 0)
		return S_OK;
	else
		return HRESULT(-1);
}

//���C���E�C���h�E�쐬
HRESULT Application::CreateMainWindow()
{
	shared_ptr<Window> pWnd = make_shared<Window>();
	pWnd->Create(m_WndClass.hInstance, m_WndClass.lpszClassName,NULL);
	m_pWndAry.push_back(pWnd);
	m_pDxBase = make_unique<Content>(pWnd->GetWindow(), m_hInstance);

	return S_OK;
}

//����������
bool Application::Init()
{
	return true;
}

//�X�V����
void Application::Update()
{
	if (!m_pDxBase)
		return;

	m_pDxBase->Init();
	
	MSG msg;
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			m_pDxBase->Release();
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			s_GrobalTimer.Tick([&]() {m_pDxBase->Update(); });
		}
	}
}

//�������
void Application::Release()
{
	pApp = NULL;
	m_pDxBase.reset();
	m_pWndAry.clear();
}