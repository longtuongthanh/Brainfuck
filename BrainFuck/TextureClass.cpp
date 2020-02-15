#include "TextureClass.h"

TextureLibrary::TextureLibrary()
{
    pDevice = 0;
}

TextureLibrary::~TextureLibrary()
{
    for (std::map<const CHAR*, ID3D11ShaderResourceView*>::iterator i = texture.begin(); i != texture.end(); ++i){
        DESTROY(i->second);
    }
    // DO NOT DESTROY DEVICE
}

RESULT TextureLibrary::Initialize(ID3D11Device* device)
{
    this->pDevice = device;
    return 0;
}

RESULT TextureLibrary::Release()
{
    delete this;
    return 0;
}

ID3D11ShaderResourceView* TextureLibrary::GetTexture(const CHAR* filename)
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
