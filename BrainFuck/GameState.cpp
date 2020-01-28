#include "GameState.h"

GameState::GameState()
{
    pShaderLib = 0;
    textureLib = 0;
}

GameState::~GameState()
{
    DESTROY(textureLib);
    for (auto i : texObject)
        DESTROY(i);
}

LONG GameState::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ShaderLibrary* shaderLib)
{
    this->pContext = context;
    this->pDevice = device;
    this->pShaderLib = shaderLib;
    BLOCKALLOC(TextureClass, textureLib);
    if (textureLib->Initialize(device)) {
        cerr << "failed to setup TextureClass\n";
        return 1;
    }

    NewTextureObject(TEXTURE_FILE);
    // else cerr << "object load success\n";
    return 0;
}

LONG GameState::Frame()
/** Animations, calculations, etc goes here.*/
{
    for (auto i : texObject)
        if (i->Frame()) {
            cerr << "warning: animation failed\n";
        }
    return 0;
}

LONG GameState::Release()
{
    delete this;
    return 0;
}

LONG GameState::Draw()
{
    // Game object render
    for (auto i : texObject)
        if (i->Render(pContext, pShaderLib->worldMatrix,
                                pShaderLib->viewMatrix,
                                pShaderLib->projectionMatrix)) {
            cerr << "warning: loading object failed\n";
            return 1;
        }
    // else cerr << "object on\n";

    return 0;
}

TextureObject* GameState::NewTextureObject(const CHAR* filename, TextureObject* target)
/** Creates a new TextureObject and add it to list of objects to render.
    You may provide a target and it will initialize that object for you
    (Used for initializing an object inheriting from TextureObject.*/
{
    if (target == NULL)
        target = new TextureObject;
    if (target == NULL) {
        cerr << "Out of memory";
        return NULL;
    }
    if (target->Initialize(pDevice, filename, textureLib, pShaderLib->GetTextureShader())) {
        DESTROY(target);
        return NULL;
    }
    texObject.insert(target);
    return target;
}
