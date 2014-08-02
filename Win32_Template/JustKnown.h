#pragma once

#include "BaseType.h"


namespace JustKnown
{
class Object;

class Window;
	class Control;

class Event;

class EventHandler;


class Object
{
};

class Window : public Object
{
public :
	bool Create(HINSTANCE, const Rect&);

	inline HWND GetHandle() const { return m_hWnd; }
	inline HWND GetParent() const { return m_hParent; }
private :
	HWND m_hWnd;
	HWND m_hParent;
};

class Event
{
};

class EventHandler
{
public :
	bool Listen(const MSG& msg);
};

}