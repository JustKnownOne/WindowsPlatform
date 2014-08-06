#include "FileEx.h"

FileInfo::FileInfo()
	: bDirectory(false)
{}
FileInfo::FileInfo(const TCHAR* _name, bool bDir)
	: bDirectory(bDir), name(_name)
{}

bool FileInfo::operator == (const std::tstring& _name) const
{
	return (name == _name);
}
FileInfo::operator std::tstring () const
{
	return name;
}
std::tostream& operator << (std::tostream& os, const FileInfo& obj)
{
	if (obj.bDirectory)
		os << _T("[");
	os << obj.name;
	if (obj.bDirectory)
		os << _T("]");

	return os;
}