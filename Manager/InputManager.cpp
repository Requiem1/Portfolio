
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

	m_PVECTOR3Pos = NULL;
	m_PVECTOR3Rot = NULL;

	m_PInputI = NULL;
	m_PInputF = NULL;

	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);


}

void InputManager::Update()
{
	if (m_PVECTOR3Pos)
	{
		if (GetKeyState('W') & 0x8000)
			m_PVECTOR3Pos->z = 1;
		else if (GetKeyState('S') & 0x8000)
			m_PVECTOR3Pos->z = -1;
		else
			m_PVECTOR3Pos->z = 0;

		if (GetKeyState('A') & 0x8000)
			m_PVECTOR3Rot->y = -1;
		else if (GetKeyState('D') & 0x8000)
			m_PVECTOR3Rot->y = 1;
		else
			m_PVECTOR3Rot->y = 0;

		if (m_PInputI != NULL && m_PInputF != NULL)
		{
			if (GetAsyncKeyState('I') & 0x8000)
				*m_PInputI = true;
			else
				*m_PInputI = false;

			if (GetAsyncKeyState('F') & 0x8000)
				*m_PInputF = true;
			else
				*m_PInputF = false;

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
			(*m_PBJump) = true;
		}

	}
	


}

void InputManager::SetPosition(D3DXVECTOR3 *_Pos, D3DXVECTOR3 *_Rot, bool * isJump)
{
	m_PVECTOR3Pos = _Pos;
	m_PVECTOR3Rot = _Rot;
	m_PBJump = isJump;
}

void InputManager::SetKeyboardInputI(bool * _InputI)
{
	m_PInputI = _InputI;
}

void InputManager::SetKeyboardInputF(bool *_InputF)
{
	m_PInputF = _InputF;
}
