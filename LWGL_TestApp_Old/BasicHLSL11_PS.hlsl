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
    row_major matrix shadowViewProjection;
    float3 posWS;
    float3 directionWS;
    float penumbraCosTheta;
    float umbraCosTheta;
    float radius;
    float3 alignmentPadding;
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
Texture2D	    g_txDiffuse : register(t0);
Texture2DArray  g_shadowMaps : register(t3);
SamplerState    g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Data buffers
//--------------------------------------------------------------------------------------
StructuredBuffer<PointLight>    g_PointLights : register(t1);
StructuredBuffer<SpotLight>     g_SpotLights : register(t2);

float ComputeDistanceAttenuation(float lightRadius, float lightDistance)
{
    // Linear attenuation
    return max(0.0f, lightRadius - lightDistance) / lightRadius;

    // Quadratic attenuation
    //const float epsilon = 0.0001f;
    //float attenuation = (lightRadius * lightRadius) / ((lightDistance * lightDistance) + epsilon);
    //attenuation *= pow(max(0.0f, 1.0f - pow(lightDistance / lightRadius, 4)), 2);
    //return attenuation;
}

float GetSpotShadowAttenuation(float4 posWS, SpotLight spot, int spotIndex)
{
    float4 posCS = mul(posWS, spot.shadowViewProjection);
    float4 posNDC = posCS / posCS.w;

    float2 shadowUV = posNDC.xy * 0.5f + 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;
    float3 shadowUVW = { shadowUV, spotIndex };

    float shadowDepth = g_shadowMaps.Sample(g_samLinear, shadowUVW).x;

    return (shadowDepth > posNDC.z) ? 1.0f : 0.0f;
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
        float shadowAttenuation = GetSpotShadowAttenuation(Input.vPosWS, spotLight, spotLightIndex);
        angleAttenuation *= angleAttenuation;

        lightIntensity += directionIntensity * distanceAttenuation * angleAttenuation * shadowAttenuation;
    }
	
    return vDiffuse * lightIntensity;
}

