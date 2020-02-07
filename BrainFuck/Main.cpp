#include "Main.h"

LRESULT Main::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch(umsg)
	{
		case WM_KEYDOWN:
			return input->keydown(wparam, lparam);

		case WM_KEYUP:
			return input->keyup(wparam, lparam);

        case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
			return input->mousechange(wparam, lparam);

		case WM_DESTROY:{
			PostQuitMessage(0);
			return 0;}

		case WM_CLOSE:{
            PostQuitMessage(0);
            return 0;}

		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

Main::Main()
{ input = NULL; graphic = NULL; gameState = NULL;}

Main::~Main()
{ DESTROY(input)
  DESTROY(graphic)
  DESTROY(gameState)}

RESULT Main::Initialize()
{
    int screenWidth = 0;
    int screenHeight = 0;
    RegisterWindow();
    InitializeWindow(screenWidth, screenHeight);

	BLOCKALLOC(Input, input);
	BLOCKCALL(input->Initialize(),
		"*** Input ***\n");

	BLOCKALLOC(Graphic, graphic);
	BLOCKCALL(graphic->Initialize(screenWidth, screenHeight, hwnd),
		"*** Graphic ***\n");

    BLOCKALLOC(GameState, gameState);
	BLOCKCALL(gameState->Initialize(graphic->GetDevice(), graphic->GetDeviceContext(), graphic->GetShaderLibrary(), input),
		"*** GameState ***\n");

    return 0;
}

RESULT Main::Release()
{
    if (graphic->Release()){
        cerr << "Graphic release failed\n";
        delete this;
        return 1;}
    graphic = NULL;

    if (input->Release()){
        cerr << "Input release failed\n";
        delete this;
        return 1;}
    input = NULL;

    ShutdownWindows();

    delete this;
    return 0;
}

RESULT Main::Run()
{
    MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
            if(msg.message == WM_QUIT)
                break;

		}
		else
			if (frame())
                return 1;
	}

    return 0;
}

RESULT Main::frame()
{
    if (gameState->Frame(input)) {
        cerr << "FRAME CALCULATION FAILED";
        return 1;
    }

    if (graphic->DrawSetup2D()) return 1;

    if (gameState->Draw()) {
        cerr << "OBJECT DRAW FAILED";
        return 1;
    }

    if (graphic->draw()){
        cerr << "RENDER FAILED\n";
        return 1;
    }
    // else cerr << "FRAME COMPLETE\n";

	BLOCKCALL(input->frame(), "CANNOT CLEAR INPUT");
    return 0;
}
