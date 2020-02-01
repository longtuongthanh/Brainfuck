#include "TextString.h"

TextString::TextString() {
	vertexBuf = 0;
	device = 0;
	texture = 0;
	shader = 0;
}
TextString::~TextString() {
	DESTROY(vertexBuf);
	// DO NOT DESTROY DEVICE, TEXTURE
}
TextString::TextString(const TextString& object) {
	vertexBuf = 0;
	texture = object.texture;
	shader = object.shader;
	string = object.string;

	Setup(object.device);
}
TextString& TextString::operator = (const TextString& object) {
	DESTROY(vertexBuf);
	// DO NOT DESTROY DEVICE

	string = object.string;

	Setup(object.device);
	texture = object.texture;
	shader = object.shader;
	return *this;
}
RESULT TextString::Setup(ID3D11Device* device)
{
	this->device = device;

	D3D11_BUFFER_DESC vertexBufDesc;

	memset(&vertexBufDesc, 0, sizeof(D3D11_BUFFER_DESC));
	// Inefficiency here

	vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufDesc.ByteWidth = sizeof(VertexType) * maxLength * 6;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	COMCALL(device->CreateBuffer(&vertexBufDesc, NULL, &vertexBuf));
	return 0;
}
RESULT TextString::Initialize(ID3D11Device* device, int maxLength,
							  TextureClass* texture, FontShader* shader)
{
	//refreshRate = rate;
	this->shader = shader;
	this->texture = texture->GetTexture(FONT_FILE);
	this->maxLength = maxLength;
	if (InitializeData()) return 1;
	if (Setup(device)) return 1;
	if (FontSetting::Initialize()) return 1;
	return 0;
}
RESULT TextString::Release() {
	delete this;
	return 0;
}
RESULT TextString::Render(ID3D11DeviceContext* deviceContext,
	D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedVertices;
	COMCALL(deviceContext->Map(vertexBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertices));
	BLOCKCALL(LoadRenderData(mappedVertices.pData), "Cannot load render data");
	deviceContext->Unmap(vertexBuf, 0);

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuf, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Shader render
	BLOCKCALL(shader->Render(deviceContext, pointCount, worldMatrix, viewMatrix, projectionMatrix, texture),
		"warning: shader render failed\n")
		// else cerr << "shader on\n";

		return 0;
}
RESULT TextString::InitializeData()
{
	string = "Hello text! No new line support.";
	size = 0.125f;
	position = Point(-0.5, 0);
	return 0;
}
RESULT TextString::Frame()
{
	return 0;
}
RESULT TextString::LoadRenderData(void* pData)
{
	VertexType* array = (VertexType*)pData;

	int length = string.length();
	pointCount = length * 6;
	if (length > maxLength) {
		cerr << "length exceed maximum allowed: " << maxLength;
		return 1;
	}
	float curx = position.x;
	for (int i = 0; i < length; i++)
	{
		char letter = string[i];
		float width = FontSetting::width(letter, size);
		float height = FontSetting::height(size);
		std::pair<float, float> xRange = FontSetting::Measurement(letter);
		array[i * 6].position = D3DXVECTOR2(curx, position.y);
		array[i * 6 + 1].position = D3DXVECTOR2(curx,
			position.y + height);
		array[i * 6 + 2].position = D3DXVECTOR2(curx + width,
			position.y + height);
		array[i * 6 + 3].position = D3DXVECTOR2(curx, position.y);
		array[i * 6 + 4].position = D3DXVECTOR2(curx + width,
			position.y + height);
		array[i * 6 + 5].position = D3DXVECTOR2(curx + width,
			position.y);
		array[i * 6].texture = D3DXVECTOR2(xRange.first, 1);
		array[i * 6 + 1].texture = D3DXVECTOR2(xRange.first, 0);
		array[i * 6 + 2].texture = D3DXVECTOR2(xRange.second, 0);
		array[i * 6 + 3].texture = D3DXVECTOR2(xRange.first, 1);
		array[i * 6 + 4].texture = D3DXVECTOR2(xRange.second, 0);
		array[i * 6 + 5].texture = D3DXVECTOR2(xRange.second, 1);

		array[i * 6].color =
			array[i * 6 + 1].color =
			array[i * 6 + 2].color =
			array[i * 6 + 3].color =
			array[i * 6 + 4].color =
			array[i * 6 + 5].color = D3DXVECTOR4(1, 1, 0, 1);

		curx += width;
	}

	return 0;
}
ID3D11ShaderResourceView* TextString::GetTexture() {
	return texture;
}

RESULT FontSetting::Initialize()
{
	while (totalWidth == 0) {
		freopen(FILENAME, "r", stdin);

		std::cin >> totalWidth >> textHeight;
		char x;
		int w;
		int v = 0;
		for (int i = 0; i < 257; i++)
			q[i] = p[i] = 0;
		while (std::cin >> std::noskipws >> x) {
			while (x == '\n') 
				std::cin >> x;
			std::cin >> std::skipws >> w;
			q[x] = w;
			p[x] = v;
			v = w;
		}
		fclose(stdin);
	}
	return 0;
}