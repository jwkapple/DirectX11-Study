#pragma once
#pragma comment(lib,"d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <d3dcompiler.h>
using namespace DirectX;
using namespace std;

class MultiShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding; // Added extra padding so structure size is multiple of 16 for Createbuffer function requirements.
	};
public:
	MultiShaderClass();
	MultiShaderClass(const MultiShaderClass&);
	~MultiShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	// 셰이더에 사용되는 변수들을 설정하고 셰이더를 이용해 준비된 모델의 정점들을 그려냅니다.
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**,XMFLOAT3,XMFLOAT4);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**,XMFLOAT3,XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;

};