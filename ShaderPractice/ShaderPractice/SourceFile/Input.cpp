#include "stdafx.h"
#include "Input.h"



InputManager::InputManager()
{
	m_KeyBoard = NULL;
	m_pDirectInput = NULL;
	InputEnd();
}


InputManager::~InputManager()
{

}

//インプット作成
void InputManager::CreateInput(HINSTANCE hInstance,HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_pDirectInput, NULL)))
	{
		return;
	}
	//キーボード
	m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyBoard, NULL);
	m_KeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_KeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//マウス
	m_pDirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
	m_Mouse->SetDataFormat(&c_dfDIMouse);
	m_Mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}

//入力開始
void InputManager::InputStart()
{
	if (m_KeyBoard != NULL)
	{
		m_KeyBoard->Acquire();
		m_IsInputStart = true;
	}
}

void InputManager::InputEnd()
{
	//入力終了
	if (m_KeyBoard != NULL)
	{
		m_KeyBoard->Unacquire();
	}
	m_IsInputStart = false;
}

//キー状態の更新
void InputManager::UpdateKeyState()
{
	memcpy_s(m_KeyStack, 256, m_KeyState, 256);

	if (m_KeyBoard != NULL)
		m_KeyBoard->GetDeviceState(256, m_KeyState);
}

//キー状態
bool InputManager::GetKeyState(BYTE KeyCode)
{
	if (m_IsInputStart != true)
		return false;

	if (m_KeyState[KeyCode] & 0x80)
	{
		return true;
	}

	return false;
}

//キーが離された時
bool InputManager::GetKeyUp(BYTE KeyCode)
{
	if (m_IsInputStart != true)
		return false;

	if (!(m_KeyState[KeyCode]) & 0x80 && m_KeyStack[KeyCode] & 0x80)
	{
		return true;
	}

	return false;
}

//キーが押された時
bool InputManager::GetKeyDown(BYTE KeyCode)
{
	if (m_IsInputStart != true)
		return false;

	if (m_KeyState[KeyCode] & 0x80 && !(m_KeyStack[KeyCode] & 0x80))
	{
		return true;
	}

	return false;
}