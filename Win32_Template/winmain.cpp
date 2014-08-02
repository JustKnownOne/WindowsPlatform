#include "prech.h"

INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, INT)
{
	JustKnown::Window wMain;

	wMain.Create(hInstance, Rect());

	JustKnown::EventHandler EventDepot;

	MSG msg;
	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;

		EventDepot.Listen(msg);
	}

	return msg.wParam;
}