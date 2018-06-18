#pragma once

#define g_INPUTMGR InputManager::GetInstance()

#define MAX_INPUT_MOUSE 8

// ���� Ŭ������ �Է¿� ���� ���� ó���Ѵ�
// ���� ó�� �����Ѱ��� Ű������ i,f,w,a,s,d,���콺 ��Ŭ��,��Ŭ��
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
	// ���� �ʱ�ȭ
	void Init();
	/*
	    ĳ������ �̵�, Ű���� IŰ�� FŰ �Է¿� ���� ó��
	*/
	void Update();
    /*
	   ĳ������ �̵� ���� ���� ����
	   ù��° ���� -> �̵�
	   �ι�° ���� -> ȸ��
	   ����° ���� -> ����
	*/
	void SetPosition(D3DXVECTOR3 *_Pos,D3DXVECTOR3 *_Rot,bool * isJump);

	// Ű���� IŰ(LŰ �ƴ�)�Է� Ȯ�ο� �� ���� ����
	void SetKeyboardInputI(bool * _InputI);
	// Ű���� FŰ �Է� Ȯ�ο� �� ���� ����
	void SetKeyboardInputF(bool *_InputF);


	// ������ ������ �ѹ��� ���� üũ�Ѵ�
	bool ButtonDown(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN; }
	// ������ ������ ��ư�� ���������� �ѹ��� üũ�Ѵ�.
	bool ButtonUp(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_UP; }
	// ������ ������ ���������� ������ �ִ��� üũ�Ѵ�. 
	bool ButtonPress(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS; }
	// ������ ������ ����Ŭ���� üũ�Ѵ�. ����� ������ ����.
	bool ButtonDB(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DBLCLK; }

	// �ٱ����� ���콺 ��ư �Է� �Լ��� ȣ���� ��� �� ������ ����
	// LBUTTON -> ���� , RBUTTON -> ������, MBUTTON->��(������ ����)
	enum INPUT_TYPE
	{
		LBUTTON,
		RBUTTON,
		MBUTTON
	};


};