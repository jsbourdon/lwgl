//--------------------------------------------------------------------------------------
// File: BasicHLSL11_VS.hlsl
//
// The vertex shader file for the BasicHLSL11 sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
cbuffer cbPerObject : register( b0 )
{
    row_major matrix g_mWorldViewProjection : packoffset( c0 );
    row_major matrix g_mWorld			    : packoffset( c4 );
};

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 vPosition	: POSITION;
	float3 vNormal		: NORMAL;
	float2 vTexcoord	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float3 vNormal		: NORMAL;
	float2 vTexcoord	: TEXCOORD0;
	float4 vPosition	: SV_POSITION;
    float4 vPosWS       : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain( VS_INPUT Input )
{
	VS_OUTPUT Output;
	
    Output.vPosWS = mul(Input.vPosition, g_mWorld);
	Output.vPosition = mul(Input.vPosition, g_mWorldViewProjection);
	Output.vNormal = mul(Input.vNormal, (float3x3)g_mWorld);
	Output.vTexcoord = Input.vTexcoord;
	
	return Output;
}

