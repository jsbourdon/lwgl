//--------------------------------------------------------------------------------------
// File: BasicHLSL11_PS.hlsl
//
// The pixel shader file for the BasicHLSL11 sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant buffers
//--------------------------------------------------------------------------------------
cbuffer cbPerFrame : register(b0)
{
    int LightCount : packoffset(c0);
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D	g_txDiffuse : register(t0);
SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Data buffers
//--------------------------------------------------------------------------------------
StructuredBuffer<float4> g_Lights : register(t1);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float3 vNormal		: NORMAL;
    float2 vTexcoord	: TEXCOORD0;
    float4 vPosition	: SV_POSITION;
    float4 vPosWS       : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain( PS_INPUT Input ) : SV_TARGET
{
	float4 vDiffuse = g_txDiffuse.Sample( g_samLinear, Input.vTexcoord );
    float lightIntensity = 0.03f;

    for (int lightIndex = 0; lightIndex < LightCount; ++lightIndex)
    {
        float4 lightInfo = g_Lights[lightIndex];
        float3 lightPosWS = lightInfo.xyz;
        float lightRadius = lightInfo.w;

        float3 lightDir = lightPosWS - Input.vPosWS.xyz;
        float lightDist = length(lightDir);
        float directionIntensity = saturate(dot(lightDir / lightDist, normalize(Input.vNormal)));

        // Linear distance attenuation
        float distanceAttenuation = max(0.0f, lightRadius - lightDist) / lightRadius;

        lightIntensity += directionIntensity * distanceAttenuation;
    }
	
    return vDiffuse * lightIntensity;
}

