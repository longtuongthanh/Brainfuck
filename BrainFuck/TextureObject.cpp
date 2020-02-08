#include "TextureObject.h"

TextureObject::TextureObject() {
    vertexBuf = 0;
    pDevice = 0;
    pointArray = 0;
    texture = 0;
    shader = 0;
}
TextureObject::~TextureObject() {
    DESTROY(vertexBuf);
    delete[] pointArray;
    // DO NOT DESTROY DEVICE, TEXTURE
}
TextureObject::TextureObject(const TextureObject& object) {
    vertexBuf = 0;
    texture = object.texture;
    shader = object.shader;

    pointCount = object.pointCount;

    pointArray = new VertexType[pointCount];
    memcpy(pointArray, object.pointArray, sizeof(VertexType) * pointCount);

    Setup(object.pDevice);
}
TextureObject& TextureObject::operator = (const TextureObject& object) {
    DESTROY(vertexBuf);
    delete[] pointArray;
    // DO NOT DESTROY DEVICE

    pointCount = object.pointCount;

    pointArray = new VertexType[pointCount];
    memcpy(pointArray, object.pointArray, sizeof(VertexType) * pointCount);

    Setup(object.pDevice);
    texture = object.texture;
    shader = object.shader;
    return *this;
}
RESULT TextureObject::Setup(ID3D11Device* device)
{
    this->pDevice = device;

    D3D11_BUFFER_DESC vertexBufDesc;
    D3D11_SUBRESOURCE_DATA vertexData;

    memset(&vertexBufDesc,0, sizeof(D3D11_BUFFER_DESC));
    memset(&vertexData,0, sizeof(D3D11_SUBRESOURCE_DATA));
    // Inefficiency here

    vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufDesc.ByteWidth = sizeof(VertexType) * pointCount;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    vertexData.pSysMem = pointArray;

    COMCALL(device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuf));
    return 0;
}

RESULT TextureObject::Initialize(ID3D11Device* device, const CHAR* textureFile, TextureLibrary* texture, TextureShader* shader)
{
    //refreshRate = rate;
    this->shader = shader;
    this->texture = texture->GetTexture(textureFile);
    if (InitializeData()) return 1;
    if (Setup(device)) return 1;
    return 0;
}
RESULT TextureObject::Release() {
    delete this;
    return 0;
}
RESULT TextureObject::Render(ID3D11DeviceContext* deviceContext,
                                     D3DXMATRIX worldMatrix,
                                     D3DXMATRIX viewMatrix,
                                     D3DXMATRIX projectionMatrix)
{

    D3D11_MAPPED_SUBRESOURCE mappedVertices;
    COMCALL(deviceContext->Map(vertexBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertices));
	BLOCKCALL(LoadRenderData(mappedVertices.pData), "cannot load data to render");
    deviceContext->Unmap(vertexBuf, 0);

    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &vertexBuf, &stride, &offset);
    deviceContext->IASetPrimitiveTopology(topology());

    // Shader render
    BLOCKCALL(shader->Render(deviceContext, pointCount, worldMatrix, viewMatrix, projectionMatrix, texture),
         "warning: shader render failed\n")
    // else cerr << "shader on\n";

    return 0;
}
RESULT TextureObject::InitializeData()
{
    pointCount = 0;

    return 0;
}
RESULT TextureObject::Frame()
{
    return 0;
}
RESULT TextureObject::LoadRenderData(void * pData)
{
	memcpy(pData, pointArray, sizeof(VertexType) * pointCount);
	return 0;
}
int TextureObject::GetPointCount() {
    return pointCount;}
ID3D11ShaderResourceView* TextureObject::GetTexture() {
    return texture;
}
