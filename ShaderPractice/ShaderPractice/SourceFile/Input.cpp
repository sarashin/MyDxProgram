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

//�C���v�b�g�쐬
void InputManager::CreateInput(HINSTANCE hInstance,HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_pDirectInput, NULL)))
	{
		return;
	}
	//�L�[�{�[�h
	m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyBoard, NULL);
	m_KeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_KeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	//�}�E�X
	m_pDirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
	m_Mouse->SetDataFormat(&c_dfDIMouse);
	m_Mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}

//���͊J�n
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
	//���͏I��
	if (m_KeyBoard != NULL)
	{
		m_KeyBoard->Unacquire();
	}
	m_IsInputStart = false;
}

//�L�[��Ԃ̍X�V
void InputManager::UpdateKeyState()
{
	memcpy_s(m_KeyStack, 256, m_KeyState, 256);

	if (m_KeyBoard != NULL)
		m_KeyBoard->GetDeviceState(256, m_KeyState);
}

//�L�[���
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

//�L�[�������ꂽ��
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

//�L�[�������ꂽ��
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