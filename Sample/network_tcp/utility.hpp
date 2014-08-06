#pragma once

template<typename T>
void SafeDelete(T*& pointer)
{
	if (pointer)
	{
		delete pointer;
		pointer = 0;
	}
}

template<typename T>
void SafeDeleteArray(T*& pointer)
{
	if (pointer)
	{
		delete [] pointer;
		pointer = 0;
	}
}

template<typename CharType>
unsigned strAlloc(CharType*& dest, const CharType* src)
{
	if (dest)
		::SafeDeleteArray(dest);

	unsigned len = 0;
	while (src[len++]);

	dest = new CharType[len];

	len = 0;
	while (dest[len++] = src[len]);

	return len;
}

template<typename Cls>
class singleton
{
protected :
	singleton(){}
	virtual ~singleton(){}

public :
	static Cls& GetReference()
	{
		static Cls instance;
		return instance;
	}
};
