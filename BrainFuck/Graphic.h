#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "useful_stuff.h"
#include "D3Dcontroller.h"
#include "TextureObject.h"
#include "ShaderLibrary.h"
#include "CameraClass.h"

const bool FULLSCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;			// Cut off everything further
const float SCREEN_NEAR = 0.1f;				// Cut off everything closer
const DXGI_SAMPLE_DESC MSAA_SETTING =
{
    8,          // Number of multisample per pixel (1,2,4,8 is guarantied)
    0           // Image quality level
}; // Multisampling on

class Graphic : private NonCopyable
{
    public:
        Graphic();
        ~Graphic();

        RESULT Initialize(int, int, HWND);
        RESULT Release();

        RESULT draw();
		/** 3D Drawing for background. Scale according to real size (keep aspect ratio).*/
        RESULT DrawSetup3D();
		/** 2D Drawing for UI elements. Scale according to screen size (stretch to fit).
			(-1, -1) is bottom left, (1, 1) is top right.*/
        RESULT DrawSetupUI();
		/** 2D Drawing for game elements. Scale according to screen width (keep aspect ratio)
			The width of the screen is -1 to 1.*/
		RESULT DrawSetup2D();

        inline ID3D11Device* GetDevice() {return controller->GetDevice();}
        inline ID3D11DeviceContext* GetDeviceContext() {return controller->GetDeviceContext();}
        inline ShaderLibrary* GetShaderLibrary() {return shaderLib;}
    protected:
    private:
        D3Dcontroller* controller;
        ShaderLibrary* shaderLib;

        int width, height;
};

#endif // GRAPHIC_H
