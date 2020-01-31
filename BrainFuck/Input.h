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
		/** Clear all temporary / one-off flags (onclick, etc.)*/
        RESULT frame();
        RESULT Release();

        RESULT keydown(WPARAM, LPARAM);
        RESULT keyup(WPARAM, LPARAM);
        RESULT mousechange(WPARAM, LPARAM);
        RESULT keyboard(char x);

		Point Mouse();
		int MouseFlag();
    protected:

    private:
        int opcode[256];
		Point mouse;
		int mouseFlag;
		/*
			0x0001: LButton down
			0x0002: RButton down
			0x0004: SHIFT down.	WARNING: UPDATE IRREGULAR
			0x0008: CTRL down	WARNING: UPDATE IRREGULAR
			0x0010: MButton down
			0x0020: XButton1 down
			0x0040: XButton2 down
			2 other bit similar but mark whether it has just changed
		*/
        RESULT exitCode;
};

#define KEY_STATE_DOWN 1
#define KEY_STATE_UP 0
#define KEY_STATE_ON_DOWN 3
#define KEY_STATE_ON_UP 2
//Note: 0x02 is flag for on event.

#endif // INPUT_H
