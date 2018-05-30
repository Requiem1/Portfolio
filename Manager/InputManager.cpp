
#include "../stdafx.h"
#include "InputManager.h"

InputManager::InputManager()
{

}
InputManager::~InputManager()
{

}

void InputManager::Init()
{
	Pos = NULL;
	Rot = NULL;

	InputI = NULL;
	InputF = NULL;
	
	UILButtonDown = NULL;
	UIRButtonDown = NULL;


	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);


}

void InputManager::Update()
{
	if (GetKeyState('W') & 0x8000)
		Pos->z = 1;
	else if (GetKeyState('S') & 0x8000)
		Pos->z = -1;
	else
		Pos->z = 0;
    
	if (GetKeyState('A') & 0x8000)
		Rot->y = -1;
	else if (GetKeyState('D') & 0x8000)
		Rot->y = 1;
	else
		Rot->y = 0;

	if (InputI != NULL && InputF != NULL)
	{
		if (GetAsyncKeyState('I') & 0x8000)
			*InputI = true;
		else
			*InputI = false;

		if (GetAsyncKeyState('F') & 0x8000)
			*InputF = true;
		else
			*InputF = false;

	}
	


	memcpy(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));

	ZeroMemory(buttonStatus, sizeof(buttonStatus));
	ZeroMemory(buttonMap, sizeof(buttonMap));

	buttonStatus[INPUT_TYPE::LBUTTON] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[INPUT_TYPE::RBUTTON] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[INPUT_TYPE::MBUTTON] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;


	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int oldStatus = buttonOldStatus[i];
		int currentStatus = buttonStatus[i];

		if (oldStatus == 0 && currentStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
		else if (oldStatus == 1 && currentStatus == 0)
			buttonMap[i] = BUTTON_INPUT_STATUS_UP;
		else if (oldStatus == 1 && currentStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
		else
			buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		(*IsJump) = true;
	}


}

void InputManager::SetPosition(D3DXVECTOR3 *_Pos, D3DXVECTOR3 *_Rot, bool * isJump)
{
	Pos = _Pos;
	Rot = _Rot;
	IsJump = isJump;
}

void InputManager::SetKeyboardInputI(bool * _InputI)
{
	InputI = _InputI;
}

void InputManager::SetKeyboardInputF(bool *_InputF)
{
	InputF = _InputF;
}
