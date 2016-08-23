#if defined _s00

float4 main(uint id : SV_VertexID) : SV_Position
{
    float2 v[3] = { float2(-1.0f, -1.0f), float2(3.0f, -1.0f), float2(-1.0f, 3.0f) };
    return float4(v[id], 0.0f, 1.0f);
}

#elif defined _s01

Texture2D<float4> s_Texture : register(t0);

[RootSignature("DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)")]
float4 main(float4 pos : SV_Position) : SV_Target0
{
    return s_Texture[pos.xy];
}

#elif defined _s02

RWTexture2D<float4> s_Target : register(u0);

[numthreads(16, 16, 1)]
[RootSignature("DescriptorTable(UAV(u0))")]
void main(uint3 global_idx : SV_DispatchThreadID)
{
    float2 pn = -1.0f + 2.0f * float2(global_idx.x, 720 - global_idx.y) / float2(1280.0f, 720.0f);
    pn.x *= 1.777f;

    float3 ro = float3(0.0f, 0.0f, 0.0f);
    float3 rd = normalize(float3(pn.x, pn.y, -2.0f));

    s_Target[global_idx.xy] = float4(rd, 1.0f);
}

#endif
