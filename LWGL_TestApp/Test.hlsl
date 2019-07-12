struct VS_OUTPUT
{
    float4 posCS    : SV_POSITION;
    float2 uv	    : TEXCOORD0;
};

float4 PSMain(VS_OUTPUT input) : SV_TARGET
{
	return float4(input.uv, 0.0f, 1.0f);
}