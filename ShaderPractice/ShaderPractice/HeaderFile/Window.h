#pragma once
#include"DxBase.h"

class WindowBase
{
	//�E�C���h�E�\�z
	CREATESTRUCT m_WndStruct;	
	//�E�C���h�E�n���h��
	HWND m_hWnd;				
protected:
	//�E�C���h�E�\�z
	virtual void PreCreate(CREATESTRUCT& pStruct, HWND Parent);
public:
	//�R���X�g���N�^
	WindowBase();	
	//�f�X�g���N�^
	~WindowBase();

	//�E�C���h�E�쐬
	bool Create(HINSTANCE hInst,const char* WndName,HWND Parent);								
	//�E�C���h�E�v���V�[�W��
	static LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);	
	//�E�C���h�E�n���h���擾
	HWND GetWindow();																			
};

class Window : public WindowBase
{

public:
	//�R���X�g���N�^
	Window();	
	//�f�X�g���N�^
	~Window();	

	//�E�C���h�E�\�z
	void PreCreate(CREATESTRUCT& pStruct, HWND Parent)override;								
	//�E�C���h�E�v���V�[�W��
	static LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};

