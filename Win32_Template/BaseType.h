#pragma once

#include <Windows.h>

struct Point : public POINT
{
	Point();
	Point(LONG _x, LONG _y);
};

struct Size : public SIZE
{
	Size();
	Size(LONG _cx, LONG _cy);
};

struct Rect : public RECT
{
	Rect();
	Rect(LONG l, LONG t, LONG r, LONG b);
	Rect(const Point& lt, const Point& rb);
};

