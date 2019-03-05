cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

Texture2D  shaderTexture[2];
SamplerState SampleType;

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

PixelInputType BumpVertexShader(VertexInputType input)
{

	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	return output;
}

float4 BumpPixelShader(PixelInputType input) : SV_TARGET
{
	
	float4 textureColor;
	float4 bumpMap;
	float3 bumpNormal;
	float3 lightDir;
	float lightIntensity;
	float4 color;


	textureColor = shaderTexture[0].Sample(SampleType, input.tex);

	bumpMap = shaderTexture[1].Sample(SampleType, input.tex);

	bumpMap = (bumpMap * 2.0f) - 1.0f;
	
	bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;

	lightDir = -lightDirection;

	lightIntensity = saturate(dot(bumpNormal, lightDir));

	color = saturate(diffuseColor * lightIntensity);

	color = color * textureColor;

	return color;
}
