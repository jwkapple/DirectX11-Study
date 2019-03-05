////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}
//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position :  POSITION;
	float4 color : COLOR;

}

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
	PIxelInputType output;

	//올바르게 행렬 연산을 하기 위하여 position 벡터를 w삽입
	input.position.w = 1.0f;

	//정점위치를 계산
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//픽셀 세이더에서 사용하기 위해 입력 색상을 저장
	output.color = input.color;

	return output;
}