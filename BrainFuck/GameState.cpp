#include "GameState.h"

GameState::GameState()
{
    pShaderLib = 0;
    textureLib = 0;
}

GameState::~GameState()
{
    DESTROY(textureLib);
    for (auto i : objects)
        DESTROY(i);
	DESTROY(camera);
	DESTROY(inputEvents);
	delete invokable1;
	delete map;
}

RESULT GameState::Initialize(ID3D11Device* device,
						   ID3D11DeviceContext* context,
						   ShaderLibrary* shaderLib,
						   Input* input)
{
    this->pContext = context;
    this->pDevice = device;
    this->pShaderLib = shaderLib;
    BLOCKALLOC(TextureLibrary, textureLib);
    BLOCKCALL(textureLib->Initialize(device), 
		"failed to setup TextureLibrary\n");

    frameTimer.Initalize();
   
	BLOCKALLOC(CameraClass, camera);
	BLOCKCALL(camera->Initialize(), 
		"cannot init camera.");

	BLOCKALLOC(EventDistributor, inputEvents);
	BLOCKCALL(inputEvents->Initialize(input),
		"cannot init input event system");

	BLOCKALLOC(InvokableMoveCameraWithArrowKey(camera, &frameTimer), invokable1);
	inputEvents->SubscribeWhileKeyDown(VK_DOWN, invokable1);
	inputEvents->SubscribeWhileKeyDown(VK_UP, invokable1);
	inputEvents->SubscribeWhileKeyDown(VK_LEFT, invokable1);
	inputEvents->SubscribeWhileKeyDown(VK_RIGHT, invokable1);

	inputEvents->Lock();

    //NewTextureObject(TEXTURE_FILE);
    map = new HexagonMap(HEXAGON_SIZE * sqrt(3) / 2, HEXAGON_SIZE, HEXAGON_PADDING);
    map->Initialize(pDevice, textureLib, pShaderLib);
    // else cerr << "object load success\n";
    debugText = new TextString();
    debugText->Initialize(pDevice, 100, textureLib, pShaderLib->GetFontShader());
    return 0;
}

RESULT GameState::Frame(Input* input)
/** Animations, calculations, etc goes here.*/
{
	cameraPos = Point(camera->position.x, camera->position.y);
    frameTimer.Mark();
    for (auto i : objects)
        if (i->Frame()) {
            cerr << "warning: animation failed\n";
        }
    debugText->ChangePosition(cameraPos);

	inputEvents->Frame();
	
	Point mouseTileCenter = map->GetCoord(input->MouseToField() + cameraPos);

    if (map->Frame(cameraPos) == 1)
    {
        debugText->InputString("error " + std::to_string((int)mouseTileCenter.x) + "," + std::to_string((int)mouseTileCenter.y));
    }
    else
    {
        debugText->InputString("keep going " + std::to_string((int)mouseTileCenter.x) + "," + std::to_string((int)mouseTileCenter.y));
    }
    //debugText->InputString(std::to_string(camera->position.x));
    return 0;
}

RESULT GameState::Release()
{
    delete this;
    return 0;
}

RESULT GameState::Draw()
{
    // Game object render
    map->Render(cameraPos, pContext, pShaderLib->worldMatrix,
        camera->viewMatrix,
        pShaderLib->projectionMatrix);

    for (auto i : objects)
        if (i->Render(pContext, pShaderLib->worldMatrix,
                                camera->viewMatrix,
                                pShaderLib->projectionMatrix)) {
            cerr << "warning: loading object failed\n";
            return 1;
        }

    debugText->Render(pContext, pShaderLib->worldMatrix, camera->viewMatrix, pShaderLib->projectionMatrix);
    // else cerr << "object on\n";
    

	BLOCKCALL(camera->Render(), "Cannot switch perspective.");

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
    objects.insert(target);
    return target;
}

TextString * GameState::NewTextString(TextString* target)
{
	if (target == NULL)
		target = new TextString;
	if (target == NULL) {
		cerr << "Out of memory";
		return NULL;
	}
	if (target->Initialize(pDevice, 100, textureLib, pShaderLib->GetFontShader())) {
		DESTROY(target);
		return NULL;
	}
	objects.insert(target);
	return target;
}

void InvokableMoveCameraWithArrowKey::Invoke(void* param)
{
	char* arg = (char*)param;
	float time = timer->GetTimeSpan();
	switch (*arg)
	{
	case VK_UP:
		camera->position.y += time;
		break;
	case VK_DOWN:
		camera->position.y -= time;
		break;
	case VK_LEFT:
		camera->position.x -= time;
		break;
	case VK_RIGHT:
		camera->position.x += time;
		break;
	default:
		break;
	}
}
