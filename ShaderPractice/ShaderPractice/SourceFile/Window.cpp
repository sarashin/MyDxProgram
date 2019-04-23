#include "stdafx.h"
#include "Window.h"
#include"Input.h"


WindowBase::WindowBase()
{
}


WindowBase::~WindowBase()
{
}

//���[�J���E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowBase::LocalWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_COMMAND:
	{
		InputManager::GetInstance().InputStart();
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEACTIVATE:
		
	default:
		InputManager::GetInstance().InputStart();
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

//�E�C���h�E�\�z
void WindowBase::PreCreate(CREATESTRUCT& pStruct, HWND Parent = NULL)
{
	pStruct.dwExStyle		= WS_EX_LEFT;
	pStruct.style			= WS_OVERLAPPEDWINDOW;
	pStruct.x				= CW_USEDEFAULT;
	pStruct.y				= CW_USEDEFAULT;
	pStruct.cx				= CW_USEDEFAULT;
	pStruct.cy				= CW_USEDEFAULT;
	pStruct.hwndParent		= Parent;
	pStruct.hMenu			= NULL;
	pStruct.lpCreateParams	= NULL;
}

//�E�C���h�E�쐬
bool WindowBase::Create(HINSTANCE hInst, const char* WindowName, HWND Parent = 0)
{
	m_WndStruct.hInstance = hInst;
	m_WndStruct.lpszClass = WindowName;

	this->PreCreate(m_WndStruct,Parent);

	m_hWnd = CreateWindowA(
							m_WndStruct.lpszClass,
							m_WndStruct.lpszClass,
							m_WndStruct.style,
							m_WndStruct.x,
							m_WndStruct.y,
							m_WndStruct.cx,
							m_WndStruct.cy,
							m_WndStruct.hwndParent,
							m_WndStruct.hMenu,
							m_WndStruct.hInstance,
							m_WndStruct.lpCreateParams
	);

	if (!m_hWnd)
		return false;

	return true;
}

//�E�C���h�E�n���h���擾
HWND WindowBase::GetWindow()
{
	return m_hWnd;
}

Window::Window()
{
}


Window::~Window()
{
}

//�E�C���h�E�\�z
void Window::PreCreate(CREATESTRUCT& pStruct, HWND Parent = NULL)
{
	pStruct.dwExStyle		= WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	pStruct.style			= WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	pStruct.x				= CW_USEDEFAULT;
	pStruct.y				= CW_USEDEFAULT;
	pStruct.cx				= CW_USEDEFAULT;
	pStruct.cy				= CW_USEDEFAULT;
	pStruct.hwndParent		= NULL;
	pStruct.hMenu			= NULL;
	pStruct.lpCreateParams	= NULL;
}

//���[�J���E�C���h�E�v���V�[�W��
LRESULT CALLBACK Window::LocalWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_KEYDOWN:
	{
		if(wParam == VK_ESCAPE){
			PostQuitMessage(0);
			break;
		}
	}
	break;
	default:
		return WindowBase::LocalWndProc(hWnd,Msg,wParam,lParam);
	}
	return 0;
}