#include "FileEx.h"

FileEx::FileEx()
	: pos(0), lineheight(25), linecount(10)
	, topline(0)
{
}
FileEx::FileEx(const TCHAR* initDir)
	: pos(0), path(initDir), lineheight(25), linecount(10)
	, topline(0)
{
	ListDirectory();
}
FileEx::~FileEx()
{
}

void FileEx::ScrollUp()
{
	pos = max(0, pos-1);
}
void FileEx::ScrollDown()
{
	pos = min(filelist.size()-1, pos+1);
}
void FileEx::Enter()
{
	std::list<FileInfo>::iterator it = filelist.begin();
	std::advance(it, pos);

	if (*it == std::tstring(_T("..")))
	{
		go_parent_dir();
		ListDirectory();
	}
	else
	{
		if (it->bDirectory)
		{
			go_child_dir(it->name.c_str());
			ListDirectory();
		}
	}
}
void FileEx::GoParent()
{
	go_parent_dir();

	ListDirectory();
}
bool FileEx::SetPos(const POINT& pt)
{
	RECT rc = {0, 0, 100, lineheight};
	::OffsetRect(&rc, 0, lineheight);

	::OffsetRect(&rc, 0, -lineheight*topline);

	int count = filelist.size();
	for (int i = 0; i < count; i++)
	{
		if (rc.top <= pt.y && pt.y < rc.bottom)
		{
			pos = i;

			return true;
		}

		::OffsetRect(&rc, 0, lineheight);
	}

	return false;
}
void FileEx::SetFont(const LOGFONT& lFont, COLORREF clr)
{
	fontinfo.lFont = lFont;
	fontinfo.color = clr;
	fontinfo.SetLoad();
}
void FileEx::GetFont(LOGFONT& lFont, COLORREF& clr) const
{
	if (fontinfo.IsLoad())
	{
		lFont = fontinfo.lFont;
		clr = fontinfo.color;
	}
}
void FileEx::SetRange(const RECT& rc)
{
	range = rc;
	linecount = (rc.bottom - rc.top) / lineheight + 1;
}
void FileEx::SetRange(const int& nMin, const int& nMax)
{
	linecount = nMax - nMin;
}
const int FileEx::GetLineHeight() const
{
	return lineheight;
}
const int FileEx::GetLineCount() const
{
	return linecount;
}
const int FileEx::GetFileCount() const
{
	return filelist.size() + 1;
}
void FileEx::SetTopLine(const int& top)
{
	topline = top;
}
void FileEx::SetCurrentPath(const TCHAR* _path)
{
	// TODO : check whether _path is valid, or not.
	WIN32_FIND_DATA fd;
	HANDLE hFile = ::FindFirstFile(_path, &fd);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = ::GetLastError();
		if (dwError == ERROR_FILE_NOT_FOUND)
		{
			// file not found.
		}

		::FindClose(hFile);
		return;
	}
	::FindClose(hFile);

	if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
	{
		// path is not directory.
		return;
	}

	std::tostringstream oss;
	oss << _path << _T("\\*");
	hFile = ::FindFirstFile(oss.str().c_str(), &fd);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = ::GetLastError();
		
		::FindClose(hFile);
		return;
	}
	::FindClose(hFile);

	path = _path;

	ListDirectory();
}
std::tstring FileEx::CurrentPath() const
{
	return path;
}
void FileEx::ListDirectory()
{
	pos = 0;
	topline = 0;

	std::tstring search(path);
	search += _T("\\*");
	WIN32_FIND_DATA fd;
	HANDLE hFile = ::FindFirstFile(search.c_str(), &fd);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = ::GetLastError();
		std::tcerr << _T("::FindFirstFile failed. : ") << dwError << std::endl;

		path = path.substr(0, path.find_last_of(_T("\\")));
		::FindClose(hFile);
		return;
	}

	filelist.clear();

	std::list<FileInfo> dirlist;
	std::list<FileInfo> normallist;
	do {

		if (fd.cFileName == std::tstring(_T(".")))
			continue;

		bool bDir = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

		if (bDir)
			dirlist.push_back(FileInfo(fd.cFileName, bDir));
		else
			normallist.push_back(FileInfo(fd.cFileName, bDir));
	} while (::FindNextFile(hFile, &fd));

	filelist.insert(filelist.cend(), dirlist.begin(), dirlist.end());
	filelist.insert(filelist.cend(), normallist.begin(), normallist.end());

	::FindClose(hFile);
}
void FileEx::go_parent_dir()
{
	path = path.substr(0, path.find_last_of(_T("\\")));
}
void FileEx::go_child_dir(const TCHAR* _name)
{
	path += _T("\\") + std::tstring(_name);
}

std::tostream& operator << (std::tostream& os, const FileEx& obj)
{
	os << obj.path << std::endl;

	std::list<FileInfo>::const_iterator it;
	int i = 0;
	for (it = obj.filelist.begin(); it != obj.filelist.end(); it++, i++)
	{
		std::tostringstream oss;
		if (i == obj.pos)
			oss << _T("=>> ");
		else
			oss << _T("    ");

		oss << *it;

		os << oss.str();
		os << std::endl;
	}
	return os;
}
HDC& operator << (HDC& dc, const FileEx& obj)
{
	int oldMode = ::SetBkMode(dc, TRANSPARENT);
	HFONT hFont, hOldFont;
	COLORREF oldFontColor;
	if (obj.fontinfo.IsLoad())
	{
		hFont = ::CreateFontIndirect(&obj.fontinfo.lFont);
		hOldFont = (HFONT)::SelectObject(dc, hFont);
		oldFontColor = ::SetTextColor(dc, obj.fontinfo.color);
	}

	RECT rc = {0,0,600,obj.lineheight};
	int format = DT_LEFT;

	std::tstring temp(obj.path);
	temp += _T("\\");
	::DrawText(dc, temp.c_str(), -1, &rc, format);
	::OffsetRect(&rc, 0, obj.lineheight);

	std::list<FileInfo>::const_iterator it;
	int i = 0;
	for (it = obj.filelist.begin(); it != obj.filelist.end(); it++, i++)
	{
		if (i < obj.topline)
			continue;

		std::tostringstream oss;
		if (i == obj.pos)
			oss << _T("=>> ");
		else
			oss << _T("    ");

		oss << *it;

		::DrawText(dc, oss.str().c_str(), -1, &rc, format);

		::OffsetRect(&rc, 0, obj.lineheight);
	}

	if (obj.fontinfo.IsLoad())
	{
		::SetTextColor(dc, oldFontColor);
		::SelectObject(dc, hOldFont);
		::DeleteObject(hFont);
	}
	::SetBkMode(dc, oldMode);

	return dc;
}
