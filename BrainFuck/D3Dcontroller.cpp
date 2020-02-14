#include "D3Dcontroller.h"

D3Dcontroller::D3Dcontroller() {
    swapchain = 0;
    device = 0;
    deviceContext = 0;
    renderTargetView = 0;
    depthStencilBuffer = 0;
    depthStencilState3D = 0;
    depthStencilState2D = 0;
    depthStencilView = 0;
    rasterState = 0;
	blendState = 0;
}
D3Dcontroller::~D3Dcontroller() {
    if (swapchain) swapchain->SetFullscreenState(false, NULL);
	DESTROY(blendState);
    DESTROY(rasterState);
    DESTROY(depthStencilView);
    DESTROY(depthStencilState3D);
    DESTROY(depthStencilState2D);
    DESTROY(depthStencilBuffer);
    DESTROY(renderTargetView);
    DESTROY(swapchain);
    DESTROY(deviceContext);
    DESTROY(device);
}
RESULT D3Dcontroller::Initialize(D3Dcontroller_setting& setting) {
    vsyncEnable = setting.vsyncEnable;
    BLOCKCALL(GetVideoCardInfo(setting.screenWidth, setting.screenHeight),
        "Failed to get video card info\n");
    BLOCKCALL(CreateSwapChain(setting.hwnd, setting.screenWidth, setting.screenHeight, setting.fullScreen, setting.msaa), 
		"Failed to create swap chain\n");
	BLOCKCALL(CreateDepthBuffer3D(setting.screenWidth, setting.screenHeight, setting.msaa), 
		"Failed to create 3D depth buffer\n");
	BLOCKCALL(CreateDepthBuffer2D(setting.screenWidth, setting.screenHeight, setting.msaa), 
		"Failed to create 2D depth buffer\n");
	BLOCKCALL(CreateRasterState(setting.screenWidth, setting.screenHeight), 
		"Failed to create raster state\n");
	BLOCKCALL(CreateMatrix(setting.screenWidth, setting.screenHeight, setting.screenNear, setting.screenDepth), 
		"Failed to create world matrix\n");
	BLOCKCALL(CreateBlendState(), 
		"Failed to create blend state\n");
    return 0;
}
RESULT D3Dcontroller::Release() {
    delete this;
    return 0;
}
RESULT D3Dcontroller::draw(float red, float green, float blue, float alpha) {
    float color[4];

    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;

    deviceContext->ClearRenderTargetView(renderTargetView, color);

	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    return 0;
}
RESULT D3Dcontroller::display() {
    if (vsyncEnable)
        return FAILED(swapchain->Present(1,0));
    else
        return FAILED(swapchain->Present(0,0));
}
ID3D11Device* D3Dcontroller::GetDevice() {
    return device;
}
ID3D11DeviceContext* D3Dcontroller::GetDeviceContext() {
    return deviceContext;
}
RefreshRate D3Dcontroller::GetRefreshRate() {
    return refreshRate;
}
RESULT D3Dcontroller::GetVideoCardInfo(int screenWidth, int screenHeight)
{
    IDXGIFactory* factory;
    IDXGIAdapter* adapter;
    IDXGIOutput* adapterOutput;
    unsigned int numModes;
    DXGI_MODE_DESC* displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;

    COMCALL(CreateDXGIFactory(IID_IDXGIFactory, (void**)&factory));
    COMCALL(factory->EnumAdapters(0, &adapter));
    COMCALL(adapter->EnumOutputs(0, &adapterOutput));
    COMCALL(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM,
                                                DXGI_ENUM_MODES_INTERLACED,
                                                &numModes,
                                                NULL));
    BLOCKALLOC(DXGI_MODE_DESC[numModes], displayModeList);
    COMCALL(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM,
                                                DXGI_ENUM_MODES_INTERLACED,
                                                &numModes,
                                                displayModeList));
    for (int i=0; i<numModes; i++)
        if (displayModeList[i].Height == screenHeight && displayModeList[i].Width == screenWidth)
            refreshRate = displayModeList[i].RefreshRate;

    COMCALL(adapter->GetDesc(&adapterDesc));
    wcstombs(videoCardDescription,adapterDesc.Description,128);

    delete[] displayModeList;
    adapterOutput->Release();
    adapter->Release();
    factory->Release();
    return 0;
}

RESULT D3Dcontroller::CreateSwapChain(HWND hwnd,
                                      int screenWidth,
                                      int screenHeight,
                                      bool fullscreen,
                                      MultisampleSetting& msaa)
{
    ID3D11Texture2D* backBuffer;
    DXGI_SWAP_CHAIN_DESC swapchainDesc = {}; //zero initialization

    swapchainDesc.BufferCount = 1;

    swapchainDesc.BufferDesc.Width = screenWidth;
    swapchainDesc.BufferDesc.Height = screenHeight;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    if (vsyncEnable)
        swapchainDesc.BufferDesc.RefreshRate = refreshRate;
    else
        swapchainDesc.BufferDesc.RefreshRate = {0, 1};
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // SCANLINE ORDER auto
    swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // SCALING auto

    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    swapchainDesc.OutputWindow = hwnd;

    swapchainDesc.SampleDesc = msaa;

    swapchainDesc.Windowed = !fullscreen;

    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // SWAP EFFECT discard

    swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // No advanced flag

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    if(FAILED(D3D11CreateDeviceAndSwapChain(NULL,
                                            D3D_DRIVER_TYPE_HARDWARE,
                                            NULL,
                                            0,
                                            &featureLevel,
                                            1,
                                            D3D11_SDK_VERSION,
                                            &swapchainDesc,
                                            &swapchain,
                                            &device,
                                            NULL,
                                            &deviceContext))){
        cerr << "Incompatible hardware\n";
        return 1;
    }
    COMCALL(swapchain->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)& backBuffer));
    COMCALL(device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView));
    backBuffer->Release();

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
    return 0;
}

RESULT D3Dcontroller::CreateDepthBuffer3D(int screenWidth,
                                      int screenHeight,
                                      MultisampleSetting& msaa)
{
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {}; // Zero initialization
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

    depthStencilBufferDesc.Width = screenWidth;
    depthStencilBufferDesc.Height = screenHeight;
    // precalculated pictures of smaller size.
    depthStencilBufferDesc.MipLevels = 1;
    // 1 for multisample texture

    depthStencilBufferDesc.ArraySize = 1;
    // Number of texture

    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.SampleDesc = msaa;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access, only GPU access
    depthStencilBufferDesc.MiscFlags = 0; // No special options

    COMCALL(device->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilBuffer));

    depthStencilDesc.DepthEnable = true; // Enable depth testing.
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true; // stencil test save time?
    depthStencilDesc.StencilReadMask = 0xff;
    depthStencilDesc.StencilWriteMask = 0xff;

    depthStencilDesc.FrontFace = FRONTFACE;
    depthStencilDesc.BackFace = BACKFACE;

    COMCALL(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState3D));
    return 0;
}

RESULT D3Dcontroller::CreateDepthBuffer2D(int screenWidth,
	int screenHeight,
	MultisampleSetting& msaa)
{
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {}; // Zero initialization
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	depthStencilBufferDesc.Width = screenWidth;
	depthStencilBufferDesc.Height = screenHeight;
	// precalculated pictures of smaller size.
	depthStencilBufferDesc.MipLevels = 1;
	// 1 for multisample texture

	depthStencilBufferDesc.ArraySize = 1;
	// Number of texture

	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc = msaa;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access, only GPU access
	depthStencilBufferDesc.MiscFlags = 0; // No special options

	COMCALL(device->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilBuffer));

	depthStencilDesc.DepthEnable = false; // Enable depth testing.
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true; // stencil test save time?
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;

	depthStencilDesc.FrontFace = FRONTFACE;
	depthStencilDesc.BackFace = BACKFACE;

	COMCALL(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState2D));
	return 0;
}

RESULT D3Dcontroller::CreateRasterState(int screenWidth, int screenHeight)
{
    D3D11_RASTERIZER_DESC rasterDesc;
    D3D11_VIEWPORT viewport;

	rasterDesc.MultisampleEnable = true; // MULTISAMPLER off
    rasterDesc.AntialiasedLineEnable = true;
    rasterDesc.CullMode = CULLMODE;
    rasterDesc.DepthBias = 0; // larger value to draw shadow to make shadows not coplane
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // Scissor test culling pixels outside of a rectangle for GUI use
    rasterDesc.ScissorEnable = false;
    /////////////////////////////////////////////////////////////////

    COMCALL(device->CreateRasterizerState(&rasterDesc, &rasterState));
    deviceContext->RSSetState(rasterState);

    viewport.Height = screenHeight;
    viewport.Width = screenWidth;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    deviceContext->RSSetViewports(1, &viewport);
    return 0;
}

RESULT D3Dcontroller::CreateMatrix(int screenWidth,
                                   int screenHeight,
                                   float screenNear,
                                   float screenDepth)
{
    float viewArc = (float)D3DX_PI / 4;
    float screenRatio = float(screenWidth) / screenHeight;
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, viewArc, screenRatio, screenNear, screenDepth);
    D3DXMatrixIdentity(&worldMatrix);
    D3DXMatrixOrthoLH(&orthoMatrix, 2, 2, screenNear, screenDepth);
	D3DXMatrixOrthoLH(&flatMatrix, 2, 2 * float(screenHeight) / screenWidth, screenNear, screenDepth);
	/*
	for (int i = 0; i < 16; i++) {
		cerr << projectionMatrix[i];
		if (i % 4 == 3)
			cerr << '\n';
		else
			cerr << ' ';
	}
	cerr << "-------------\n";

	for (int i = 0; i < 16; i++) {
		cerr << orthoMatrix[i];
		if (i % 4 == 3)
			cerr << '\n';
		else
			cerr << ' ';
	}
	cerr << "-------------\n";

	for (int i = 0; i < 16; i++) {
		cerr << worldMatrix[i];
		if (i % 4 == 3)
			cerr << '\n';
		else
			cerr << ' ';
	}
	// */

    return 0;
}


RESULT D3Dcontroller::CreateBlendState()
{
	D3D11_BLEND_DESC blendDesc;

	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;

	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	BLOCKCALL(device->CreateBlendState(&blendDesc, &blendState), "cannot create blend state");
	deviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
	return 0;
}

void D3Dcontroller::TurnZBufferOn()
{
    deviceContext->OMSetDepthStencilState(depthStencilState3D, 1);
}

void D3Dcontroller::TurnZBufferOff()
{
    deviceContext->OMSetDepthStencilState(depthStencilState2D, 1);
}
