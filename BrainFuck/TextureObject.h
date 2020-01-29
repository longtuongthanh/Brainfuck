#ifndef INGAME_3D_TEXTURE_OBJECT_H
#define INGAME_3D_TEXTURE_OBJECT_H

#include "useful_stuff.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "TextureClass.h"
#include "TextureShader.h"
#include "GraphicObject.h"

static const CHAR* TEXTURE_FILE = "texture.dds";

class TextureObject : public GraphicObject
{
    public:
        struct VertexType
        {
            D3DXVECTOR3 position;
            D3DXVECTOR2 texture;
        };
    public:
        TextureObject();
        TextureObject(const TextureObject&);
        virtual TextureObject& operator = (const TextureObject&);
        virtual ~TextureObject();

        RESULT Initialize(ID3D11Device*, const CHAR*, TextureClass*, TextureShader*);
        virtual RESULT Release();
        RESULT Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

        virtual RESULT InitializeData();
        virtual RESULT Frame();

        int GetPointCount();
        ID3D11ShaderResourceView* GetTexture();
        inline VertexType& operator[](int x)
        /** Denoted counterclockwise */
        {return pointArray[ (x*2 < pointCount) ? x*2 : (pointCount - x)*2 - 1];}
    protected:
        RESULT Setup(ID3D11Device*);

		virtual D3D11_PRIMITIVE_TOPOLOGY topology()
		{return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;}
        int pointCount;
        VertexType* pointArray;

        ID3D11ShaderResourceView* texture;
        
    private:
        ID3D11Buffer *vertexBuf;

        ID3D11Device* device;
        CHAR* imageSrc;
        TextureShader* shader;
        //RefreshRate refreshRate;
};

#endif // INGAME_3D_TEXTURE_OBJECT_H
