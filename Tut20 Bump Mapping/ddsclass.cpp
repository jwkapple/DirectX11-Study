#include "ddsclass.h"

DDSClass::DDSClass()
{
	mSrvArray[0] = 0;
	mSrvArray[1] = 0;
	mSrvArray[2] = 0;
}

DDSClass::DDSClass(const DDSClass&)
{

}


DDSClass::~DDSClass()
{

}

bool DDSClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t** filename)
{
	HRESULT result;

	//result = CreateDDSTextureFromFile(device, filename, nullptr, &mSrv);
	result = CreateDDSTextureFromFileEx(device,filename[0], 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, (ID3D11Resource**)&mTexture, &mSrvArray[0], nullptr);
	if (FAILED(result))
	{
		return false;
	}

	result = CreateDDSTextureFromFileEx(device, filename[1], 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false, (ID3D11Resource**)&mTexture, &mSrvArray[1], nullptr);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DDSClass::Shutdown()
{
	if (mTexture)
	{
		mTexture->Release();
		mTexture = 0;
	}

	if (mSrvArray)
	{
		mSrvArray[0]->Release();
		mSrvArray[1]->Release();

		delete[] mSrvArray;
	}
}
ID3D11ShaderResourceView** DDSClass::GetTextureArray()
{
	return mSrvArray;
}