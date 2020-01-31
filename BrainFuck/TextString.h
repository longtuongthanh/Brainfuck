#pragma once

#undef __in
#include <utility>
#include <string>
#include "useful_stuff.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "TextureClass.h"
#include "FontShader.h"
#include "GraphicObject.h"


class TextString : public GraphicObject
{
	struct VertexType
	{
		D3DXVECTOR2 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR4 color;
	};
	static constexpr const CHAR* FONT_FILE = "font.png";
public:
	TextString();
	TextString(const TextString&);
	virtual TextString& operator = (const TextString&);
	virtual ~TextString();

	RESULT Initialize(ID3D11Device*, int, TextureClass*, FontShader*);
	virtual RESULT Release();
	RESULT Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	virtual RESULT InitializeData();
	virtual RESULT Frame();
	virtual RESULT LoadRenderData(void*);

	ID3D11ShaderResourceView* GetTexture();
protected:
	RESULT Setup(ID3D11Device*);

	std::string string;
	float size;
	int maxLength;
	Point position;
	int pointCount;

	ID3D11ShaderResourceView* texture;
private:
	ID3D11Buffer *vertexBuf;

	ID3D11Device* device;
	CHAR* imageSrc;
	FontShader* shader;
	//RefreshRate refreshRate;
};

#include <iostream>
namespace FontSetting {
	static constexpr const char* FILENAME = "char_size.txt";
	static int totalWidth = 0;
	static int textHeight = 0;
	static int p[257] = {};
	static int q[257] = {};
	// p: x-coord before letter, q: x-coord after letter
	extern RESULT Initialize();
	static inline std::pair<float, float> Measurement(char x)
	{
		return std::pair<float, float>(float(p[x]) / totalWidth, float(q[x]) / totalWidth);
	}
	static inline float width(char x, float multiplier = 1.0) {
		return (q[x] - p[x]) * multiplier / textHeight;
	}
	static inline float height(float multiplier = 1.0) {
		return multiplier;
	}
};
