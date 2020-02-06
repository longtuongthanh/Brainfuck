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
}

LONG GameState::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ShaderLibrary* shaderLib)
{
    this->pContext = context;
    this->pDevice = device;
    this->pShaderLib = shaderLib;
    BLOCKALLOC(TextureClass, textureLib);
    BLOCKCALL(textureLib->Initialize(device), 
		"failed to setup TextureClass\n");

    frameTimer.Initalize();
   
	BLOCKALLOC(CameraClass, camera);
	BLOCKCALL(camera->Initialize(), 
		"cannot init camera.");
    //NewTextureObject(TEXTURE_FILE);
	
    map = new HexagonMap(0.15, 0.2, 0.01);
    map->Initialize(pDevice, textureLib, pShaderLib);
    // else cerr << "object load success\n";
    debugText = new TextString();
    debugText->Initialize(pDevice, 100, textureLib, pShaderLib->GetFontShader());
    sound = new SoundClass();
    
    return 0;
}

LONG GameState::Frame(Input* input)
/** Animations, calculations, etc goes here.*/
{
    frameTimer.Mark();
    float time = frameTimer.GetTimeSpan();
    for (auto i : objects)
        if (i->Frame()) {
            cerr << "warning: animation failed\n";
        }
    debugText->ChangePosition(Point(camera->position.x, camera->position.y));

    if (input->keyboard(VK_LEFT) == KEY_STATE_DOWN || input->keyboard(VK_LEFT) == KEY_STATE_ON_DOWN)
    {
        sound->LoadWaveFile("gwyn.wav");
        sound->PlayWaveFile();
        camera->position.x -= time;
    }
    if (input->keyboard(VK_RIGHT) == KEY_STATE_DOWN || input->keyboard(VK_RIGHT) == KEY_STATE_ON_DOWN)
    {
        camera->position.x += time;
    }
    if (input->keyboard(VK_UP) == KEY_STATE_DOWN || input->keyboard(VK_UP) == KEY_STATE_ON_DOWN)
    {
        camera->position.y += time;
    }
    if (input->keyboard(VK_DOWN) == KEY_STATE_DOWN || input->keyboard(VK_DOWN) == KEY_STATE_ON_DOWN)
    {
        camera->position.y -= time;
    }
    if (map->Frame(camera) == 1)
    {
        debugText->InputString("error " + std::to_string(camera->position.x) + "," + std::to_string(camera->position.y));
    }
    else
    {
        debugText->InputString("keep going " + std::to_string(camera->position.x) + "," + std::to_string(camera->position.y));
    }
    //debugText->InputString(std::to_string(camera->position.x));
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
    map->Render(pContext, pShaderLib->worldMatrix,
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
