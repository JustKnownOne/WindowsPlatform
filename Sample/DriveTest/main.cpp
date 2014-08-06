#include <iostream>
#include <conio.h>
#include <sstream>

#include <Windows.h>
#include <tchar.h>

#if defined(UNICODE) || defined(_UNICODE)
#define tcout wcout
#define tstring wstring
#define tcerr wcerr
#define tostringstream wostringstream
#else
#define tcout cout
#define tstring string
#define tcerr cerr
#define tostringstream ostringstream
#endif

#define HAVE(a,opt) ((a & opt) == opt)

int main(void)
{
	DWORD dwDrive = ::GetLogicalDrives();
	if (dwDrive == 0)
	{
		DWORD dwError = ::GetLastError();
		std::tcerr << _T("::GetLogicalDrives failed : ") << dwError << std::endl;

		_getch();
		return 0;
	}

	std::tcout << _T("::GetLogicalDrives : ") << std::hex << dwDrive << std::dec << std::endl;
	int bitcount = sizeof(DWORD)*8;
	for (int i = 0; i < bitcount; i++)
	{
		if (((dwDrive >> i) & 0x01) == 0x01)
		{
			std::tcout << TCHAR(_T('A') + i) << _T(" drive exist.") << std::endl;

			std::tostringstream oss;
			oss << TCHAR(_T('A') + i) << _T(":\\");
			UINT uRet = ::GetDriveType(oss.str().c_str());
			if (uRet == DRIVE_UNKNOWN)
				std::tcout << _T("\tunknown type drive.") << std::endl;
			else if (uRet == DRIVE_NO_ROOT_DIR)
				std::tcout << _T("\tdrive have no root dir.") << std::endl;
			else if (uRet == DRIVE_REMOVABLE)
				std::tcout << _T("\tdrive is removable.") << std::endl;
			else if (uRet == DRIVE_FIXED)
				std::tcout << _T("\tdrive is fixed.") << std::endl;
			else if (uRet == DRIVE_REMOTE)
				std::tcout << _T("\tdrive is remote.") << std::endl;
			else if (uRet == DRIVE_CDROM)
				std::tcout << _T("\tdrive is CD-ROM drive.") << std::endl;
			else if (uRet == DRIVE_RAMDISK)
				std::tcout << _T("\tdrive is ramdisk.") << std::endl;
		}
	}




	_getch();
	return 0;
}