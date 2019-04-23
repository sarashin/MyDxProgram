#include"stdafx.h"
#include"Application.h"
#include"GrobalTimer.h"

#define D3DX_PI ((FLOAT) 3.141592654f) 
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))

//実行アプリケーション
AppBase* pApp;

//グローバルタイマー
GrobalTimer& s_GrobalTimer = GrobalTimer::GetInstance();

//コンストラクタ
AppBase::AppBase() 
{
	pApp = this;
}

//デストラクタ
AppBase::~AppBase()
{

}

//ウインドウのグローバルプロシージャ
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

		//ローカルのウインドウプロシージャへ処理を委託
		for (int i = 0; i < Size; i++)
		{
			Window* WndBase = (*pWnd)[i].get();
			if (WndBase)
				return WndBase->LocalWndProc(hWnd, Msg, wParam, lParam);
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}


//ウインドウの定義
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

//メインウインドウ作成
HRESULT AppBase::CreateMainWindow()
{
	shared_ptr<Window> pWnd = make_shared<Window>();
	pWnd->Create(m_WndClass.hInstance, "MainWindow",NULL);
	m_pWndAry.push_back(pWnd);

	return S_OK;
}

//更新処理
void AppBase::Update()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//アプリケーションの動き
HRESULT AppBase::Run(HINSTANCE hInst)
{
	HRESULT hr = S_OK;
	m_hInstance = hInst;

	//ウインドウの鋳型作成
	if(FAILED(this->RegistWndClass()))
	{
		hr = HRESULT(-1);
		return hr;
	}

	//メインウインドウ作成
	if(FAILED(this->CreateMainWindow()))
	{
		hr = HRESULT(-1);
		return hr;
	}

	//初期化
	if (!this->Init())
	{
		hr = HRESULT(-1);
		return hr;
	}

	//更新
	this->Update();

	//解放
	this->Release();
}

//インスタンスハンドル取得
HINSTANCE AppBase::GetHInstance()
{
	return m_hInstance;
}

//ウインドウクラスの取得
void AppBase::GetWindowClass(unique_ptr<WNDCLASSEX> WndClass)
{
	*WndClass = m_WndClass;
}

//ウインドウのポインタ取得
vector<shared_ptr<Window>>* AppBase::GetWndPtr()
{
	return &m_pWndAry;
}

//コンストラクタ
Application::Application()
{
	pApp = this;
}

//デストラクタ
Application::~Application()
{

}

//ウインドウの定義
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

//メインウインドウ作成
HRESULT Application::CreateMainWindow()
{
	shared_ptr<Window> pWnd = make_shared<Window>();
	pWnd->Create(m_WndClass.hInstance, m_WndClass.lpszClassName,NULL);
	m_pWndAry.push_back(pWnd);
	m_pDxBase = make_unique<Content>(pWnd->GetWindow(), m_hInstance);

	return S_OK;
}

//初期化処理
bool Application::Init()
{
	return true;
}

//更新処理
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

//解放処理
void Application::Release()
{
	pApp = NULL;
	m_pDxBase.reset();
	m_pWndAry.clear();
}