#ifndef SHADERLIBRARY_H
#define SHADERLIBRARY_H

#include "TextureShader.h"
#include "FontShader.h"

class ShaderLibrary
{
    public:
        ShaderLibrary();
        ~ShaderLibrary();

        RESULT Initialize(ID3D11Device*, HWND);
        RESULT Release();

        TextureShader* GetTextureShader();
		FontShader* GetFontShader();
        D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
    protected:

    private:
        TextureShader* texShader;
		FontShader* fontShader;
};

#endif // SHADERLIBRARY_H
