#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetSpecularPower(float);
	void SetSpecularColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	float GetSpecularPower();
	XMFLOAT4 GetSpecularColor();
	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:

	float m_specularPower;
	XMFLOAT4 m_specularColor;
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
};