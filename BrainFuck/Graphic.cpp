#include "Graphic.h"

Graphic::Graphic()
{
    controller = NULL;
    shaderLib = NULL;
}

Graphic::~Graphic()
{
    DESTROY(controller);
    DESTROY(shaderLib);
}

RESULT Graphic::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    height = screenHeight;
    width = screenWidth;

    BLOCKALLOC(D3Dcontroller, controller);

    D3Dcontroller_setting setting = {width, height, VSYNC_ENABLED, hwnd, FULLSCREEN, MSAA_SETTING, SCREEN_DEPTH, SCREEN_NEAR};
    if (controller->Initialize(setting)) {
        cerr << "*** D3D controller ***\n";
        return 1;
    }
    // else cerr << "D3D controller load success\n";
    BLOCKALLOC(ShaderLibrary, shaderLib);
    if (shaderLib->Initialize(controller->GetDevice(), hwnd)) {
        cerr << "failed to load shader library\n";
        return 1;
    }
    // else cerr << "shader load success\n";

    return 0;
}

RESULT Graphic::draw()
{
    if (controller->display()) {
        cerr << "warning: cannot display\n";
        return 1;
    }
    // else cerr << "display complete\n";

    return 0;
}

RESULT Graphic::DrawSetup3D()
{
	controller->TurnZBufferOn();

	shaderLib->worldMatrix = controller->worldMatrix;
	shaderLib->projectionMatrix = controller->projectionMatrix;

    if (controller->draw()) {
        cerr << "warning: cannot clear screen\n";
        return 1;
    }
    // else cerr << "clear complete\n";

    return 0;
}

RESULT Graphic::DrawSetupUI()
{
	shaderLib->worldMatrix = controller->worldMatrix;
	shaderLib->projectionMatrix = controller->orthoMatrix;

	controller->TurnZBufferOff();

    return 0;
}

RESULT Graphic::DrawSetup2D()
{
	shaderLib->worldMatrix = controller->worldMatrix;
	shaderLib->projectionMatrix = controller->flatMatrix;

	controller->TurnZBufferOff();

	return 0;
}

RESULT Graphic::ClearScreen()
{
	if (controller->draw()) {
		cerr << "warning: cannot clear screen\n";
		return 1;
	}
	// else cerr << "clear complete\n";
	return 0;
}

RESULT Graphic::Release()
{
    if (controller->Release()){
        cerr << "*** Graphic ***\n";
        delete this;
        return 1;}
    controller = NULL;

    delete this;
    return 0;
}
