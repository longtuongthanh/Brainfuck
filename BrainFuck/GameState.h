#ifndef GAMESTATE_H
#define GAMESTATE_H

#undef __in
#include <unordered_set>
#include "useful_stuff.h"
#include "TextureObject.h"
#include "ShaderLibrary.h"
#include "TextureClass.h"
#include "TextString.h"
#include "CameraClass.h"
#include "HexagonMap.h"
#include "Timer.h"
#include "Input.h"
#include "SoundClass.h"
#include "EventDistributor.h"
#include "TestWorldMatrix.h"

// Hexagon size moved to HexagonTile.h

class InvokableMoveCameraWithArrowKey;

class GameState final : private NonCopyable
{
    public:
        RESULT Frame(Input*);
        TextureObject* NewTextureObject(const CHAR* filename, TextureObject* target = NULL);
		TextString* NewTextString(TextString* target = NULL);
        // documentary moved to cpp file
    public:
        GameState();
        ~GameState();
        RESULT Initialize(ID3D11Device*, ID3D11DeviceContext*, ShaderLibrary*, Input*);
        RESULT Release();
        RESULT Draw();
		RESULT DrawUI();
    protected:
        std::unordered_set<GraphicObject*> objects;
    private:
        TextureLibrary* textureLib;
        ShaderLibrary* pShaderLib;
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pContext;
        HexagonMap* map;
        Timer frameTimer;
        CameraClass* camera;
        TextString* debugText;
        Sound* sound;
		EventDistributor* inputEvents;
		InvokableMoveCameraWithArrowKey* invokable1;
        TestDragable* testWorldMatrix;

		// GetCoord, GetLocation moved to HexagonMap

		Point cameraPos;
};

class InvokableMoveCameraWithArrowKey : public Invokable {
public:
	InvokableMoveCameraWithArrowKey(CameraClass* camera, Timer* timer): camera(camera), timer(timer){}
	void Invoke(void*); // arguement is char*
private:
	CameraClass* camera;
	Timer* timer;
};



#endif // GAMESTATE_H
