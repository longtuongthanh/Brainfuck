#include "TextureClass.h"

TextureClass::TextureClass()
{
    pDevice = 0;
}

TextureClass::~TextureClass()
{
    for (std::map<const CHAR*, ID3D11ShaderResourceView*>::iterator i = texture.begin(); i != texture.end(); ++i){
        DESTROY(i->second);
    }
    // DO NOT DESTROY DEVICE
}

RESULT TextureClass::Initialize(ID3D11Device* device)
{
    this->pDevice = device;
    return 0;
}

RESULT TextureClass::Release()
{
    delete this;
    return 0;
}

ID3D11ShaderResourceView* TextureClass::GetTexture(const CHAR* filename)
{
    std::map<const CHAR*, ID3D11ShaderResourceView*>::iterator trg = texture.find(filename);
    if (trg == texture.end()){
        ID3D11ShaderResourceView* newTex;
		if (FAILED(D3DX11CreateShaderResourceViewFromFile(pDevice, filename, NULL, NULL, &newTex, NULL)))
		{
			cerr << "cannot load texture " << filename;
			return NULL;
		}
        return texture[filename] = newTex;
    }
    else
        return trg->second;
}
