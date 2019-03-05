#pragma once

#include <d3d11.h>
#include <DDSTextureLoader.h>
#include <fstream>

using namespace DirectX;

class DDSClass
{
public:
	DDSClass();
	DDSClass(const DDSClass&);
	~DDSClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const wchar_t**);
	void Shutdown();
	bool Render(ID3D11Device*, ID3D11DeviceContext*);

	ID3D11ShaderResourceView** GetTextureArray();
private:
	ID3D11ShaderResourceView* mSrvArray[2];

	ID3D11Texture2D* mTexture;
};
