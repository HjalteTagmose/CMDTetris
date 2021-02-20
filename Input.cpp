#include "Input.h"
#include "map"
#include <WinUser.h>

map<char, bool> held;

bool Input::OnKeyHeld(unsigned char key)
{
	return (0x8000 & GetAsyncKeyState(key)) != 0;
}

bool Input::OnKeyPressed(unsigned char key)
{
	if (OnKeyHeld(key))
	{
		if (!held[key])
		{
			held[key] = true;
			return true;
		}
	}
	else held[key] = false;
	return false;
}
