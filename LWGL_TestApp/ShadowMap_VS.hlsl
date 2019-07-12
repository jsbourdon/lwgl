//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
cbuffer cbPerObject : register(b0)
{
    row_major matrix g_mWorldView : packoffset( c0 );
    row_major matrix g_mProjection : packoffset(c4);
};

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 vPosition	: POSITION;
};

struct VS_OUTPUT
{
	float4 vPosition	: SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
	VS_OUTPUT Output;

    float4 posVS = mul(Input.vPosition, g_mWorldView);
    posVS.xyz += normalize(posVS.xyz) * 10.0f; // Depth bias

    Output.vPosition = mul(posVS, g_mProjection);
	
	return Output;
}

