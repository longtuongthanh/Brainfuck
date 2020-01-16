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

RESULT Input::keydown(WPARAM wparam, LPARAM)
// warning: may be thread-unsafe, need buffer
{
    if (wparam < 256)
        opcode[wparam] = true;
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
    opcode[wparam] = false;
    cerr << char(254);
    return 0;
}

RESULT Input::mousemove(WPARAM wparam, LPARAM lparam)
{
    mouse.x = GET_X_LPARAM(lparam);
    mouse.y = GET_Y_LPARAM(lparam);
    return 0;
}
