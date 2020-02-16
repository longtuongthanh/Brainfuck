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
	DESTROY(map);
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
    BLOCKALLOC(HexagonMap, map);
    BLOCKCALL(map->Initialize(pDevice, textureLib, pShaderLib),"cannot initialize map");
    // else cerr << "object load success\n";
    debugText = new TextString();
    debugText->Initialize(pDevice, 100, textureLib, pShaderLib->GetFontShader());
    sound = new Sound();
    sound->LoadWaveFile("gwyn.wav");
    //sound->PlayWaveFile();

    testWorldMatrix = new TestDragable();
    testWorldMatrix->Initialize(pDevice, TEXTURE_FILE, textureLib, pShaderLib->GetTextureShader());
    return 0;
}

RESULT GameState::Frame(Input* input)
/** Animations, calculations, etc goes here.*/
{
	cameraPos = Point(camera->position.x, camera->position.y);
    frameTimer.Mark();

    float time = frameTimer.GetTimeSpan();
    for (auto i : objects)
        if (i->Frame()) {
            cerr << "warning: animation failed\n";
        }
    debugText->ChangePosition(cameraPos);

	inputEvents->Frame();
	
	//Point x = GetCoord(input->MouseToField() + cameraPos);
    Point x = input->MouseToScreen() + cameraPos;
    if (input->keyboard(VK_LEFT) == KEY_STATE_DOWN || input->keyboard(VK_LEFT) == KEY_STATE_ON_DOWN)
    {
        //camera->position.x -= frameTimer.GetTimeSpan();
    }
    if (input->keyboard(VK_RIGHT) == KEY_STATE_DOWN || input->keyboard(VK_RIGHT) == KEY_STATE_ON_DOWN)
    {
        //camera->position.x += frameTimer.GetTimeSpan();
    }
    if (input->keyboard(VK_UP) == KEY_STATE_DOWN || input->keyboard(VK_UP) == KEY_STATE_ON_DOWN)
    {
        //camera->position.y += frameTimer.GetTimeSpan();
    }
    if (input->keyboard(VK_DOWN) == KEY_STATE_DOWN || input->keyboard(VK_DOWN) == KEY_STATE_ON_DOWN)
    {
        //camera->position.y -= frameTimer.GetTimeSpan();
    }
    if (map->Frame(Point(camera->position.x, camera->position.y)) == 1)
    {
        debugText->InputString("error " + std::to_string((float)x.x) + "," + std::to_string((float)x.y));
    }
    else
    {
        debugText->InputString("" + std::to_string((float)x.x) + "," + std::to_string((float)x.y));
    }
    testWorldMatrix->Frame(*input, cameraPos);
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

    // else cerr << "object on\n";
    testWorldMatrix->MakeWorldMatrix();
    testWorldMatrix->Render(pContext, *testWorldMatrix->pMatWorld, camera->viewMatrix, pShaderLib->projectionMatrix);
    
    debugText->Render(pContext, pShaderLib->worldMatrix, camera->viewMatrix, pShaderLib->projectionMatrix);

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
