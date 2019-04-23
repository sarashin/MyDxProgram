#pragma once
#include"DxBase.h"

class WindowBase
{
	//ウインドウ構築
	CREATESTRUCT m_WndStruct;	
	//ウインドウハンドル
	HWND m_hWnd;				
protected:
	//ウインドウ構築
	virtual void PreCreate(CREATESTRUCT& pStruct, HWND Parent);
public:
	//コンストラクタ
	WindowBase();	
	//デストラクタ
	~WindowBase();

	//ウインドウ作成
	bool Create(HINSTANCE hInst,const char* WndName,HWND Parent);								
	//ウインドウプロシージャ
	static LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);	
	//ウインドウハンドル取得
	HWND GetWindow();																			
};

class Window : public WindowBase
{

public:
	//コンストラクタ
	Window();	
	//デストラクタ
	~Window();	

	//ウインドウ構築
	void PreCreate(CREATESTRUCT& pStruct, HWND Parent)override;								
	//ウインドウプロシージャ
	static LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};

