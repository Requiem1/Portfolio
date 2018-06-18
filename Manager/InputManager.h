#pragma once

#define g_INPUTMGR InputManager::GetInstance()

#define MAX_INPUT_MOUSE 8

// 현재 클래스는 입력에 관한 것을 처리한다
// 현재 처리 가능한것은 키보드의 i,f,w,a,s,d,마우스 좌클릭,우클릭
class InputManager
{
	SINGLETON(InputManager);
private:
	D3DXVECTOR3 *m_PVECTOR3Pos;
	D3DXVECTOR3 *m_PVECTOR3Rot;

	bool *m_PBJump;

	bool *m_PInputI;
	bool *m_PInputF;

	bool *m_PUILButtonDown;
	bool *m_PUIRButtonDown;


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


	// 프레임 단위로 한번만 눌림 체크한다
	bool ButtonDown(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN; }
	// 프레임 단위로 버튼이 떼어졌는지 한번만 체크한다.
	bool ButtonUp(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_UP; }
	// 프레임 단위로 지속적으로 누르고 있는지 체크한다. 
	bool ButtonPress(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS; }
	// 프레임 단위로 더블클릭을 체크한다. 현재는 쓰이지 않음.
	bool ButtonDB(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DBLCLK; }

	// 바깥에서 마우스 버튼 입력 함수를 호출할 경우 쓸 열거형 변수
	// LBUTTON -> 왼쪽 , RBUTTON -> 오른쪽, MBUTTON->휠(쓰이지 않음)
	enum INPUT_TYPE
	{
		LBUTTON,
		RBUTTON,
		MBUTTON
	};


};