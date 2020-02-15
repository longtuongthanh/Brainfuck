#include "ShaderLibrary.h"

ShaderLibrary::ShaderLibrary()
{
    texShader = 0;
}

ShaderLibrary::~ShaderLibrary()
{
    DESTROY(texShader);
}

RESULT ShaderLibrary::Initialize(ID3D11Device* device, HWND hwnd)
{
    BLOCKALLOC(TextureShader, texShader);
    BLOCKCALL(texShader->Initialize(device, hwnd),"failed to load texture shader\n");
	BLOCKALLOC(FontShader, fontShader);
	BLOCKCALL(fontShader->Initialize(device, hwnd), "failed to load font shader\n");

    return 0;
}

RESULT ShaderLibrary::Release()
{
    delete this;
    return 0;
}

TextureShader* ShaderLibrary::GetTextureShader()
{
    return texShader;
}

FontShader * ShaderLibrary::GetFontShader()
{
	return fontShader;
}
