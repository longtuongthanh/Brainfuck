#include "Input.h"

Input::Input()
{
    //ctor
}

Input::~Input()
{
    //dtor
}

RESULT Input::Initialize()
{
    for (int i=0; i<256; i++)
        opcode[i] = false;
    exitCode = 0;
    return 0;
}

RESULT Input::frame()
{
	for (int i = 0; i < 256; i++)
		opcode[i] &= -3;
	mouseFlag &= 0x00ff;
    return exitCode;
}

RESULT Input::Release()
{
    delete this;
    return 0;
}

RESULT Input::keyboard(char x)
{
    return opcode[x];
}

Point Input::Mouse()
{
	return mouse;
}

int Input::MouseFlag()
{
	return mouseFlag;
}

RESULT Input::keydown(WPARAM wparam, LPARAM)
// warning: may be thread-unsafe, need buffer
{
    if (wparam < 256)
        opcode[wparam] = KEY_STATE_ON_DOWN;
    else
        cerr << "Key not captured " << wparam << '\n';

    switch (wparam)
    {
    case VK_ESCAPE:
        exitCode = CODE_KILL;
        break;
    default:
        break;
    }

    cerr << char(wparam);
    return 0;
}

RESULT Input::keyup(WPARAM wparam, LPARAM)
{
    opcode[wparam] = KEY_STATE_ON_UP;
    cerr << char(254);
    return 0;
}

RESULT Input::mousechange(WPARAM wparam, LPARAM lparam)
{
    mouse.x = GET_X_LPARAM(lparam);
    mouse.y = GET_Y_LPARAM(lparam);

	if (mouseFlag != wparam)
	{
		mouseFlag = wparam | (((mouseFlag ^ wparam) & 0x00ff) << 8);
	}

    return 0;
}
