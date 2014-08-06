#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const int lineheight = 25;
	static FileEx fe(_T("D:"));
	static HWND hEditPath = 0;

	if (uMsg == WM_CREATE)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		fe.SetRange(rc);

		::UpdateScrollbar(hWnd, fe);

		return 0;
	}
	else if (uMsg == WM_SIZE)
	{
		RECT rc = {0, 0, 0, 0};
		rc.right = LOWORD(lParam);
		rc.bottom = HIWORD(lParam);

		fe.SetRange(rc);

		::UpdateScrollbar(hWnd, fe);

		return 0;
	}
	else if (uMsg == WM_VSCROLL)
	{
		int pos = ::GetScrollPos(hWnd, SB_VERT);
		if (LOWORD(wParam) == SB_THUMBPOSITION || LOWORD(wParam) == SB_THUMBTRACK)
		{
			pos = HIWORD(wParam);
			::SetScrollPos(hWnd, SB_VERT, pos, TRUE);
		}
		else if (LOWORD(wParam) == SB_TOP)
		{
			pos = 0;
			::SetScrollPos(hWnd, SB_VERT, 0, TRUE);
		}
		else if (LOWORD(wParam) == SB_BOTTOM)
		{
			int nMax = fe.GetFileCount();
			pos = nMax;
			::SetScrollPos(hWnd, SB_VERT, nMax, TRUE);
		}
		else if (LOWORD(wParam) == SB_ENDSCROLL)
		{
		}
		else if (LOWORD(wParam) == SB_LINEDOWN)
		{
			int nMin, nMax;
			::GetScrollRange(hWnd, SB_VERT, &nMin, &nMax);
			pos = min(nMax, pos + 1);
			::SetScrollPos(hWnd, SB_VERT, pos, TRUE);
		}
		else if (LOWORD(wParam) == SB_LINEUP)
		{
			int nMin, nMax;
			::GetScrollRange(hWnd, SB_VERT, &nMin, &nMax);
			pos = max(nMin, pos - 1);
			::SetScrollPos(hWnd, SB_VERT, pos, TRUE);
		}
		else if (LOWORD(wParam) == SB_PAGEDOWN)
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			::GetScrollInfo(hWnd, SB_VERT, &si);
			pos = min(si.nMax, pos + si.nPage/2);
			::SetScrollPos(hWnd, SB_VERT, pos, TRUE);
		}
		else if (LOWORD(wParam) == SB_PAGEUP)
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			::GetScrollInfo(hWnd, SB_VERT, &si);
			pos = max(si.nMin, pos - si.nPage/2);
			::SetScrollPos(hWnd, SB_VERT, pos, TRUE);
		}

		fe.SetTopLine(pos);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		hdc << fe;

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		fe.SetPos(ptMouse);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDBLCLK)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		int width = rc.right - rc.left;

		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		int lineheight = fe.GetLineHeight();
		if (ptMouse.y > 0 && ptMouse.y < lineheight)
		{
			std::tostringstream oss;
			oss << fe.CurrentPath() << _T("\\");

			if (hEditPath && ::IsWindow(hEditPath))
			{
				::SetWindowText(hEditPath, oss.str().c_str());
			}
			else
			{
				hEditPath = ::CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), oss.str().c_str(), WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
					0, 0, width, lineheight, hWnd, (HMENU)IDC_EDITPATH, (HINSTANCE)::GetModuleHandle(NULL), NULL);

				::SetWindowSubclass(hEditPath, EditProc, SUBCLASS_EDIT, (DWORD_PTR)hWnd);
			}

			::ShowWindow(hEditPath, SW_SHOW);
			::SetFocus(hEditPath);
		}

		if (!fe.SetPos(ptMouse))
			return 0;

		fe.Enter();

		fe.SetRange(rc);

		::UpdateScrollbar(hWnd, fe);

		::InvalidateRect(hWnd, &rc, TRUE);
		return 0;
	}
	else if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_UP)
		{
			fe.ScrollUp();
		}
		else if (wParam == VK_DOWN)
		{
			fe.ScrollDown();
		}
		else if (wParam == VK_RETURN)
		{
			fe.Enter();

			RECT rc;
			::GetClientRect(hWnd, &rc);
			fe.SetRange(rc);

			UpdateScrollbar(hWnd, fe);
		}
		else if (wParam == VK_BACK)
		{
			fe.GoParent();

			RECT rc;
			::GetClientRect(hWnd, &rc);
			fe.SetRange(rc);

			UpdateScrollbar(hWnd, fe);
		}
		else if (wParam == VK_ESCAPE)
		{
			::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
		return 0;
	}
	else if (uMsg == WM_MOUSEWHEEL)
	{
		//HIWORD(wParam)
		short diff = GET_WHEEL_DELTA_WPARAM(wParam)/WHEEL_DELTA;
		int pos = ::GetScrollPos(hWnd, SB_VERT);
		int nMin, nMax;
		::GetScrollRange(hWnd, SB_VERT, &nMin, &nMax);
		pos = min(max(pos - diff, nMin), nMax);
		::SetScrollPos(hWnd, SB_VERT, pos, TRUE);

		fe.SetTopLine(pos);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_RBUTTONDOWN)
	{
		LOGFONT lfont;
		COLORREF color;
		fe.GetFont(lfont, color);

		CHOOSEFONT chFont;
		::memset(&chFont, 0, sizeof(chFont));
		chFont.lpLogFont = &lfont;
		chFont.hwndOwner = hWnd;
		chFont.lStructSize = sizeof(chFont);
		chFont.Flags = CF_APPLY | CF_EFFECTS;
		chFont.rgbColors = color;
		chFont.iPointSize = lfont.lfHeight*10;

		if (::ChooseFont(&chFont))
		{
			fe.SetFont(lfont, chFont.rgbColors);

			RECT rc;
			::GetClientRect(hWnd, &rc);
			::InvalidateRect(hWnd, &rc, TRUE);
		}

		return 0;
	}
	//else if (uMsg == WM_COMMAND)
	//{
	//	if (LOWORD(wParam) == IDOK)
	//	{
	//		HWND hPrevWnd = ::GetFocus();
	//		bool IsEditCtrl = false;

	//		TCHAR classname[100];
	//		::GetClassName(hPrevWnd, classname, 100);

	//		if (_tcscmp(classname, _T("Edit")) == 0)
	//			IsEditCtrl = true;

	//		if (IsEditCtrl)
	//		{
	//			TCHAR path[MAX_PATH];
	//			::GetWindowText(hPrevWnd, path, MAX_PATH);
	//			fe.SetCurrentPath(path);
	//		}
	//	}

	//	return 0;
	//}
	else if (uMsg == WM_UPDATE_PATH)
	{
		std::tstring path = *(std::tstring*)wParam;
		fe.SetCurrentPath(path.c_str());

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void UpdateScrollbar(HWND hWnd, const FileEx& fe)
{
	int filecount = fe.GetFileCount();
	int linecount = fe.GetLineCount();

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = filecount;
	si.nPos = 0;
	si.nPage = linecount;
	::SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}

LRESULT CALLBACK EditProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,UINT_PTR uIdSubclass,DWORD_PTR dwRefData)
{
	static std::tstring path;
	if (uMsg == WM_GETDLGCODE)
	{
		return (DLGC_WANTALLKEYS | ::CallWindowProc(WndProc, hWnd, uMsg, wParam, lParam));
	}
	else if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_RETURN)
		{
			::ShowWindow(hWnd, SW_HIDE);
			HWND hParent = (HWND)dwRefData;
			TCHAR buffer[MAX_PATH];
			::GetWindowText(hWnd, buffer, MAX_PATH);
			path = buffer;
			::PostMessage(hParent, WM_UPDATE_PATH, (WPARAM)&path, 0);
			return 0;
		}
		else
		{
			return ::CallWindowProc(WndProc, hWnd, uMsg, wParam, lParam);
		}
	}
	else if (uMsg == WM_CHAR)
	{
		if (wParam == VK_RETURN)
			return 0;
	}

	return ::DefSubclassProc(hWnd,uMsg,wParam,lParam);
}
