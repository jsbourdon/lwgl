//--------------------------------------------------------------------------------------
// File: BasicHLSL11_PS.hlsl
//
// The pixel shader file for the BasicHLSL11 sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float3 vNormal		: NORMAL;
    float2 vTexcoord	: TEXCOORD0;
    float4 vPosition	: SV_POSITION;
    float4 vPosWS       : TEXCOORD1;
};

struct PointLight
{
    float3 posWS;
    float radius;
};

struct SpotLight
{
    float3 posWS;
    float3 directionWS;
    float penumbraCosTheta;
    float umbraCosTheta;
    float radius;
};

//--------------------------------------------------------------------------------------
// Constant buffers
//--------------------------------------------------------------------------------------
cbuffer cbPerFrame : register(b0)
{
    int g_PointLightCount : packoffset(c0.x);
    int g_SpotLightCount : packoffset(c0.y);
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D	g_txDiffuse : register(t0);
SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Data buffers
//--------------------------------------------------------------------------------------
StructuredBuffer<PointLight> g_PointLights : register(t1);
StructuredBuffer<SpotLight> g_SpotLights : register(t2);

float ComputeDistanceAttenuation(float lightRadius, float lightDistance)
{
    // Linear attenuation
    return max(0.0f, lightRadius - lightDistance) / lightRadius;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain( PS_INPUT Input ) : SV_TARGET
{
	float4 vDiffuse = g_txDiffuse.Sample( g_samLinear, Input.vTexcoord );
    float3 normalizedNormalWS = normalize(Input.vNormal);
    float lightIntensity = 0.03f;

    for (int pointLightIndex = 0; pointLightIndex < g_PointLightCount; ++pointLightIndex)
    {
        PointLight pointLight = g_PointLights[pointLightIndex];
        float3 lightPosWS = pointLight.posWS;
        float lightRadius = pointLight.radius;

        float3 lightDir = lightPosWS - Input.vPosWS.xyz;
        float lightDist = length(lightDir);
        float directionIntensity = saturate(dot(lightDir / lightDist, normalize(Input.vNormal)));
        float distanceAttenuation = ComputeDistanceAttenuation(lightRadius, lightDist);

        lightIntensity += directionIntensity * distanceAttenuation;
    }

    for (int spotLightIndex = 0; spotLightIndex < g_SpotLightCount; ++spotLightIndex)
    {
        SpotLight spotLight = g_SpotLights[spotLightIndex];
        float3 lightPosWS = spotLight.posWS;
        float lightRadius = spotLight.radius;

        float3 lightDir = lightPosWS - Input.vPosWS.xyz;
        float lightDist = length(lightDir);
        float3 normalizedLightDir = lightDir / lightDist;
        
        float directionIntensity = saturate(dot(normalizedLightDir, normalize(Input.vNormal)));
        float distanceAttenuation = ComputeDistanceAttenuation(lightRadius, lightDist);
        float angleAttenuation = saturate((dot(-normalizedLightDir, spotLight.directionWS) - spotLight.umbraCosTheta) / (spotLight.penumbraCosTheta - spotLight.umbraCosTheta));
        angleAttenuation *= angleAttenuation;

        lightIntensity += directionIntensity * distanceAttenuation * angleAttenuation;
    }
	
    return vDiffuse * lightIntensity;
}

