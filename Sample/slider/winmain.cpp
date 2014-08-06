#include "windefine.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, INT nCmdShow)
{
	INITCOMMONCONTROLSEX cce;
	cce.dwSize = sizeof(cce);
	::InitCommonControlsEx(&cce);

	LPCTSTR szClassName = _T("TestApplica");

	WNDCLASSEX wcex;
	::memset(&wcex, 0, sizeof(wcex));

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = &WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(MAINICON));
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassName;
	wcex.hIconSm = ::LoadIcon(hInst, MAKEINTRESOURCE(MAINICON));

	if (!::RegisterClassEx(&wcex))
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	HWND hWnd = ::CreateWindowEx(0, szClassName, _T("Eternal"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,NULL,hInst,NULL);
	if (hWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 2;
	}

	::ShowWindow(hWnd, nCmdShow);
	::UpdateWindow(hWnd);

	MSG msg;

	while (::GetMessage(&msg, 0,0,0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return msg.wParam;
}
