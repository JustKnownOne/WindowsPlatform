#include "FileEx.h"

FontInfo::FontInfo()
	: bLoad(false)
{
	std::fstream fontfile;

	fontfile.open("font.cfg", std::ios_base::in | std::ios_base::binary);

	if (!fontfile.is_open())
		return;

	fontfile.read((char*)&lFont, sizeof(LOGFONT));
	fontfile.read((char*)&color, sizeof(COLORREF));

	bLoad = true;
	fontfile.close();
}
FontInfo::~FontInfo()
{
	if (bLoad)
	{
		std::fstream fontfile;

		fontfile.open("font.cfg", std::ios_base::out | std::ios_base::binary);

		fontfile.write((const char*)&lFont, sizeof(LOGFONT));
		fontfile.write((const char*)&color, sizeof(COLORREF));

		fontfile.close();
	}
}
bool FontInfo::IsLoad() const
{
	return bLoad;
}
void FontInfo::SetLoad()
{
	bLoad = true;
}
