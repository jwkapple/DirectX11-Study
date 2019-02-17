#include "lightclass.h"

LightClass::LightClass()
{

}

LightClass::LightClass(const LightClass&)
{

}

LightClass::~LightClass()
{

}

void LightClass::SetSpecularPower(float specularPower)
{
	m_specularPower = specularPower;
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}
void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

