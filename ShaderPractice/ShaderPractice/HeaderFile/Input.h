#pragma once
//#include<dinput.h>
#pragma comment(lib, "dinput8.lib")
//#include<d3d11.h>

//キー入力を読み込むクラス
class InputManager : public Singleton<InputManager>
{
private:
	//シングルトン
	friend class Singleton<InputManager>;
	//DirectInput
	LPDIRECTINPUT8			m_pDirectInput;
	//デバイス
	LPDIRECTINPUTDEVICE8	m_KeyBoard;		
	LPDIRECTINPUTDEVICE8	m_Mouse;
	//キー状態
	BYTE					m_KeyState[256];	
	//前のキー状態
	BYTE					m_KeyStack[256];	
	//キー入力状態か
	bool					m_IsInputStart;
	//コンストラクタ
	//コピー禁止の為private
	InputManager(const InputManager&) {}
protected:
	InputManager();
public:
	~InputManager();
	//インプット作成
	void CreateInput(HINSTANCE hInstance,HWND hWnd);
	//インプット開始
	void InputStart();
	//インプット終了
	void InputEnd();
	//キー状態の更新
	void UpdateKeyState();
	//キーが押されたか
	bool GetKeyState(BYTE KeyCode);
	//キーを押し込んだか
	bool GetKeyDown(BYTE KeyCode);
	//キーを離したか
	bool GetKeyUp(BYTE KeyCode);
};

