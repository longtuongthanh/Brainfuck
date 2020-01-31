#ifndef GAMESTATE_H
#define GAMESTATE_H

#undef __in
#include <unordered_set>
#include "useful_stuff.h"
#include "TextureObject.h"
#include "ShaderLibrary.h"
#include "TextureClass.h"
#include "TextString.h"
#include "HexagonMap.h"
#include "Timer.h"

class GameState
{
    public:
        virtual RESULT Frame();
        TextureObject* NewTextureObject(const CHAR* filename, TextureObject* target = NULL);
		TextString* NewTextString(TextString* target = NULL);
        // documentary moved to cpp file
    public:
        GameState();
        ~GameState();
        RESULT Initialize(ID3D11Device*, ID3D11DeviceContext*, ShaderLibrary*);
        RESULT Release();
        RESULT Draw();
    protected:
        std::unordered_set<GraphicObject*> objects;
    private:
        TextureClass* textureLib;
        ShaderLibrary* pShaderLib;
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pContext;
        HexagonMap* map;
        Timer frameTimer;
};

#endif // GAMESTATE_H
