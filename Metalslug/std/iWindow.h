#pragma once

#include <windows.h>
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif // _DEBUG

#define GDIPVER 0x0110
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;
void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height);
wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(wchar_t* str);
