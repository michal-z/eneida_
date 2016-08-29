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

float2 ComplexMul(float2 a, float2 b)
{
    return float2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

float2 ComplexSq(float2 a)
{
    return float2(a.x * a.x - a.y * a.y, 2 * a.x * a.y);
}

[numthreads(8, 8, 1)]
[RootSignature("DescriptorTable(UAV(u0))")]
void main(uint3 dispatch_tid : SV_DispatchThreadID)
{
    float2 z = float2(0.0f, 0.0f);
    float2 dz = float2(1.0f, 0.0f);
    float d = 0.0f;

    float2 c = -1.0f + 2.0f * float2(dispatch_tid.x, 1024 - dispatch_tid.y) / float2(1024.0f, 1024.0f);
    c.x -= 0.5f;

    for (int i = 0; i < 256; ++i)
    {
        dz = 2.0f * ComplexMul(z, dz) + float2(1.0f, 0.0f);
        z = ComplexSq(z) + c;

        float m2 = dot(z, z);
        if (m2 > 100.0f)
        {
            d = sqrt(m2 / dot(dz, dz)) * 0.5f * log(m2);
            break;
        }
    }

    d = 1.2f * pow(abs(d), 0.3f);
    s_Target[dispatch_tid.xy] = float4(d, d, d, 1.0f);
}

#endif
