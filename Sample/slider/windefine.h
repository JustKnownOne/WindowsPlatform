#pragma once

#pragma comment(lib, "../FileEx/FileEx.lib")

#define WINVER				0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE			0x0600

#pragma comment(lib, "comctl32.lib")

#include "../FileEx/FileEx.h"
#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>
#include "resource.h"

enum CONTROL_ID
{
	IDC_EDITPATH = 102,
};

enum SUBCLASS_ID
{
	SUBCLASS_EDIT = 1,
};

enum WM_USER_ID
{
	WM_UPDATE_PATH = WM_USER + 1,
};


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

void UpdateScrollbar(HWND hWnd, const FileEx& fe);

LRESULT CALLBACK EditProc(HWND,UINT,WPARAM,LPARAM,UINT_PTR,DWORD_PTR);