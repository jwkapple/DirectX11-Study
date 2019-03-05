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

Texture2D  shaderTexture[3];
SamplerState SampleType;

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType LightVertexShader(VertexInputType input)
{

	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	return output;
}

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	
	float4 color;
	float3 lightDir;
	float lightIntensity;
	float4 alphaValue;
	float4 color1;
	float4 color2;
	float4 blendColor;

	color1 = shaderTexture[0].Sample(SampleType, input.tex);

	color2 = shaderTexture[1].Sample(SampleType, input.tex);

	alphaValue = shaderTexture[2].Sample(SampleType, input.tex);

	blendColor = (alphaValue * color1) + ((1.0f - alphaValue) * color2);

	lightDir = -lightDirection;

	lightIntensity = saturate(dot(input.normal, lightDir));

	color = saturate(diffuseColor * lightIntensity);

	color = color * blendColor;

	return color;
}
