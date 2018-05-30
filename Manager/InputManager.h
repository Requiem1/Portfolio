#pragma once

#define g_pINPUTMGR InputManager::GetInstance()

#define MAX_INPUT_MOUSE 8

class InputManager
{
	SINGLETON(InputManager)
private:
	D3DXVECTOR3 *Pos;
	D3DXVECTOR3 *Rot;

	bool *IsJump;

	bool *InputI;
	bool *InputF;

	bool *UILButtonDown;
	bool *UIRButtonDown;


	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];
	byte buttonMap[MAX_INPUT_MOUSE];

	enum INPUT_STATUS
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};
	


public:
	// 변수 초기화
	void Init();
	/*
	    캐릭터의 이동, 키보드 I키와 F키 입력에 관한 처리
	*/
	void Update();
    /*
	   캐릭터의 이동 관련 변수 설정
	   첫번째 인자 -> 이동
	   두번째 인자 -> 회전
	   세번째 인자 -> 점프
	*/
	void SetPosition(D3DXVECTOR3 *_Pos,D3DXVECTOR3 *_Rot,bool * isJump);

	// 키보드 I키(L키 아님)입력 확인에 쓸 변수 설정
	void SetKeyboardInputI(bool * _InputI);
	// 키보드 F키 입력 확인에 쓸 변수 설정
	void SetKeyboardInputF(bool *_InputF);



	bool ButtonDown(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN; }
	bool ButtonUp(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_UP; }
	bool ButtonPress(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS; }
	bool ButtonDB(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DBLCLK; }

	enum INPUT_TYPE
	{
		LBUTTON,
		RBUTTON,
		MBUTTON
	};


};