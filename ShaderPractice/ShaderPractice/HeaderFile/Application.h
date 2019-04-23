#pragma once
#include"Window.h"

//アプリケーションクラス
class AppBase
{
private:
protected:
	//インスタンスハンドル
	HINSTANCE m_hInstance;
	//ウインドウクラス
	WNDCLASSEX m_WndClass;
	//ウインドウ配列
	vector<shared_ptr<Window>> m_pWndAry;	
	//ウインドウクラスに情報をセット
	virtual HRESULT RegistWndClass();		
	//ウインドウクラス作成
	virtual HRESULT CreateMainWindow();		

	//初期化
	virtual bool	Init()=0;		
	//更新
	virtual void	Update();				
	//解放
	virtual void	Release()=0;			
public:
	//コンストラクタ
	AppBase();			
	//デストラクタ
	virtual ~AppBase(); 

	//アプリケーションを実行
	HRESULT Run(HINSTANCE hInst);			
	//グローバルウインドウプロシージャ
	static LRESULT CALLBACK GrobalWndProc( HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
	//インスタンスハンドルの取得
	HINSTANCE GetHInstance();				
	//ウインドウクラス取得
	void GetWindowClass(unique_ptr<WNDCLASSEX> pWndClass);
	//ウインドウ取得
	vector<shared_ptr<Window>>* GetWndPtr();
};

class Application:public AppBase
{
public:
	//アプリケーション
	unique_ptr<DxBase> m_pDxBase;			

	//コンストラクタ
	Application();							
	//デストラクタ
	~Application();							
	//初期化
	bool	Init();					
	//更新
	void	Update();			
	//ウインドウクラスに情報をセット
	HRESULT RegistWndClass()override;		
	//ウインドウクラス作成
	HRESULT CreateMainWindow()override;		
	//解放
	void	Release();					
};
