R"(
    struct VS_OUTPUT
    {
        float4 posCS    : SV_POSITION;
        float2 uv	    : TEXCOORD0;
    };

    VS_OUTPUT VSMain(uint vertexId : SV_VertexID)
    {
        const float4 vertex0 = { -3, -1, 0, 1 };
        const float4 vertex1 = { 1,  3, 0, 1 };
        const float4 vertex2 = { 1, -1, 0, 1 };

        const float2 uv0 = { -1,  1 };
        const float2 uv1 = { 1, -1 };
        const float2 uv2 = { 1,  1 };

        const float4 vertices[] = { vertex0, vertex1, vertex2 };
        const float2 uvs[] = { uv0, uv1, uv2 };

        VS_OUTPUT output;

        output.posCS = vertices[vertexId];
        output.uv = uvs[vertexId];

        return output;
    }
)"
