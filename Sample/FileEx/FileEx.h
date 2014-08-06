#pragma once

#ifdef FILEEX_EXPORTS
#define FILEEXAPI __declspec(dllexport)
#else
#define FILEEXAPI __declspec(dllimport)
#endif

#pragma warning(disable:4251)

#include <Windows.h>
#include <tchar.h>

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <fstream>

#if defined(UNICODE) || defined(_UNICODE)
#define tstring wstring
#define tostringstream wostringstream
#define tostream wostream
#define tcerr wcerr
#else
#define tstring string
#define tostringstream ostringstream
#define tostream ostream
#define tcerr cerr
#endif


struct FILEEXAPI FileInfo
{
	std::tstring name;
	bool bDirectory;

	FileInfo();
	FileInfo(const TCHAR* _name, bool bDir);

	bool operator == (const std::tstring& _name) const;
	operator std::tstring () const;
	friend FILEEXAPI std::tostream& operator << (std::tostream& os, const FileInfo& obj);
};

struct FILEEXAPI DriveInfo
{
	std::string name;

	DriveInfo();
	~DriveInfo();

	void EnumDirve();
};

struct FILEEXAPI FontInfo
{
	LOGFONT lFont;
	COLORREF color;

	FontInfo();
	~FontInfo();

	bool IsLoad() const;
	void SetLoad();
private :
	bool bLoad;
};

enum key_id
{
	SPECIAL	= 0xE0,
	UP			= 0x48,
	DOWN		= 0x50,
	ENTER		= 0x0D,
	ESC		= 0x1B,
};

class FILEEXAPI FileEx
{
public :
	FileEx();
	FileEx(const TCHAR* initDir);
	~FileEx();

	void ChangeDrive(const DriveInfo& drive);
	void ScrollUp();
	void ScrollDown();
	void Enter();
	void GoParent();
	bool SetPos(const POINT& pt);
	void SetFont(const LOGFONT& lFont, COLORREF clr);
	void GetFont(LOGFONT& lFont, COLORREF& clr) const;
	void SetRange(const RECT& rc);
	void SetRange(const int& nMin, const int& nMax);

	const int GetLineHeight() const;
	const int GetLineCount() const;
	const int GetFileCount() const;

	void SetTopLine(const int& top);

	std::tstring CurrentPath() const;
	void SetCurrentPath(const TCHAR* _path);

	friend FILEEXAPI std::tostream& operator << (std::tostream& os, const FileEx& obj);
	friend FILEEXAPI HDC& operator << (HDC& dc, const FileEx& obj);
private :
	void ListDirectory();
	void go_parent_dir();
	void go_child_dir(const TCHAR* _name);
private :
	std::tstring path;
	std::list<FileInfo> filelist;
	int pos;
	int lineheight;
	int linecount;
	RECT range;
	FontInfo fontinfo;

	int topline;
};
