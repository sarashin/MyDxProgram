#include "stdafx.h"
#include "Application.h"
#pragma comment(lib,"d3d11.lib")

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Application App;
	App.Run(hInst);
}