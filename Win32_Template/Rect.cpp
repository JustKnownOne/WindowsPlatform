#include "prech.h"
#include "BaseType.h"

Rect::Rect()
{
	left = top = right = bottom = 0;
}

Rect::Rect(LONG l, LONG t, LONG r, LONG b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

Rect::Rect(const Point& lt, const Point& rb)
{
	left = lt.x;
	top = lt.y;
	right = rb.x;
	bottom = rb.y;
}