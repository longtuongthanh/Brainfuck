#ifndef INPUT_H
#define INPUT_H

#include "useful_stuff.h"
#include "windowsx.h"

class Input : private NonCopyable
{
    public:
        Input();
        ~Input();

        RESULT Initialize();
        RESULT frame();
        RESULT Release();

        RESULT keydown(WPARAM, LPARAM);
        RESULT keyup(WPARAM, LPARAM);
        RESULT mousemove(WPARAM, LPARAM);
        RESULT keyboard(char x);
    protected:

    private:
        bool opcode[256];
        Point mouse;
        RESULT exitCode;
};

#endif // INPUT_H
