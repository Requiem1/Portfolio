// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <d3dx9.h>
#include <assert.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <set>
#include <map>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <atlstr.h>
using namespace std;
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")



#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif 

#define SAFE_WNDPROC(p) if(p) {(p)->WndProc(hWnd,message,wParam,lParam);}
#define SAFE_UPDATE(p) if(p) { (p)->Update();}
#define SAFE_RENDER(p) if(p) { (p)->Render();}
#define SAFE_RELEASE(p) if(p) { (p)->Release(); (p) = NULL;}
#define SAFE_DELETE(p) if(p) { delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p) if (p) { delete[](p); (p) = NULL; }
#define SAFE_ADDREF(p) if (p) { (p)->AddRef(); }
#define SAFE_NEW(p, DataType) { if(!p) p = new DataType; ZeroMemory(p, sizeof(DataType); }

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

extern HWND g_hWnd;

#include "Base/BaseObject.h"
#include "GlobalDefinition.h"
#include "Manager/DeviceManager.h"
#include "Camera/Camera.h"
#include "Manager/InputManager.h"
#include "Utility/CollisionDetection.h"
#include "Base/IDisplayObject.h"
#include "Manager/DisplayObjManager.h"
#include "Manager/FontManager.h"
#include "Manager/TextureManager.h"
#include "Manager/SceneManager.h"
#include "Manager/MapManager.h"
#include "Manager/PlayerManager.h"
#include "Manager/EnemyManager.h"
#include "Manager/SoundManager.h"

#include "Manager/TimeManager.h"
#include "Utility/D3DUtility.h"
#include "Utility/Debug.h"
#include "Application.h"