#pragma once
//#include<dinput.h>
#pragma comment(lib, "dinput8.lib")
//#include<d3d11.h>

//�L�[���͂�ǂݍ��ރN���X
class InputManager : public Singleton<InputManager>
{
private:
	//�V���O���g��
	friend class Singleton<InputManager>;
	//DirectInput
	LPDIRECTINPUT8			m_pDirectInput;
	//�f�o�C�X
	LPDIRECTINPUTDEVICE8	m_KeyBoard;		
	LPDIRECTINPUTDEVICE8	m_Mouse;
	//�L�[���
	BYTE					m_KeyState[256];	
	//�O�̃L�[���
	BYTE					m_KeyStack[256];	
	//�L�[���͏�Ԃ�
	bool					m_IsInputStart;
	//�R���X�g���N�^
	//�R�s�[�֎~�̈�private
	InputManager(const InputManager&) {}
protected:
	InputManager();
public:
	~InputManager();
	//�C���v�b�g�쐬
	void CreateInput(HINSTANCE hInstance,HWND hWnd);
	//�C���v�b�g�J�n
	void InputStart();
	//�C���v�b�g�I��
	void InputEnd();
	//�L�[��Ԃ̍X�V
	void UpdateKeyState();
	//�L�[�������ꂽ��
	bool GetKeyState(BYTE KeyCode);
	//�L�[���������񂾂�
	bool GetKeyDown(BYTE KeyCode);
	//�L�[�𗣂�����
	bool GetKeyUp(BYTE KeyCode);
};

