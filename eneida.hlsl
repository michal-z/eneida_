struct Params
{
    uint frame;
};
ConstantBuffer<Params> s_Params;
//==============================================================================
#if defined _s00

float4 main(uint id : SV_VertexID) : SV_Position
{
    float2 v[3] = { float2(-1.0f, -1.0f), float2(3.0f, -1.0f), float2(-1.0f, 3.0f) };
    return float4(v[id], 0.0f, 1.0f);
}
//==============================================================================
#elif defined _s01

#define RootSig \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL)"

Texture2D<float4> s_Texture : register(t0);

[RootSignature(RootSig)]
float4 main(float4 pos : SV_Position) : SV_Target0
{
    return s_Texture[pos.xy];
}
//==============================================================================
#elif defined _s02

#define RootSig \
    "DescriptorTable(UAV(u0))"

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
[RootSignature(RootSig)]
void main(uint3 dispatch_tid : SV_DispatchThreadID)
{
    float2 z = float2(0.0f, 0.0f);
    float2 dz = float2(1.0f, 0.0f);
    float d = 0.0f;

    float2 c = -1.0f + 2.0f * float2(dispatch_tid.x, 1024.0f - dispatch_tid.y) / float2(1024.0f, 1024.0f);
    c.x -= 0.5f;

    for (int i = 0; i < 256; ++i)
    {
        dz = 2.0f * ComplexMul(z, dz) + float2(1.0f, 0.0f);
        z = ComplexSq(z) + c;

        float m2 = dot(z, z);
        if (m2 > 100.0f)
        {
            d = sqrt(m2 / dot(dz, dz)) * log(m2);
            break;
        }
    }

    d = pow(abs(d), 0.3f);
    s_Target[dispatch_tid.xy] = float4(d, d, d, 1.0f);
}
//==============================================================================
#elif defined _s03

#define kViewDistance 50.0f
#define RootSig \
    "DescriptorTable(UAV(u0))"

RWTexture2D<float4> s_Target : register(u0);

float4 Map(float3 p)
{
    float4 obj = float4(1.0f, 0.0f, 0.0f, p.y + 2.0f);

    float d = length(p - float3(0.1f, 0.0f, -5.0f)) - 1.0f;
    if (d < obj.w) obj = float4(0.0f, 1.0f, 0.0f, d);

    d = length(p - float3(1.1f, 0.0f, -4.0f)) - 0.5f;
    if (d < obj.w) obj = float4(0.0f, 0.0f, 1.0f, d);

    d = length(p - float3(-1.5f, 0.0f, -3.5f)) - 0.25f;
    if (d < obj.w) obj = float4(1.0f, 1.0, 0.0f, d);

    return obj;
}

float MapD(float3 p)
{
    float dist = min(p.y + 2.0f, length(p - float3(0.1f, 0.0f, -5.0f)) - 1.0f);
    dist = min(dist, length(p - float3(1.1f, 0.0f, -4.0f)) - 0.5f);
    dist = min(dist, length(p - float3(-1.5f, 0.0f, -3.5f)) - 0.25f);
    return dist;
}

float4 Intersect(float3 ro, float3 rd)
{
    float dist = 0.001f;

    for (;;)
    {
        float4 obj = Map(ro + dist * rd);
        if (obj.w < 0.001f || dist > kViewDistance) return float4(obj.r, obj.g, obj.b, dist);
        dist += obj.w;
    }
}

float Shadow(float3 ro, float3 rd)
{
    float dist = 0.001f;
    float res = 1.0f;

    for (;;)
    {
        float d = MapD(ro + dist * rd);
        if (d < 0.001f) return 0.0f;
        res = min(res, 8.0f * d / dist);
        if (dist > kViewDistance) return res;
        dist += d;
    }
}

float3 ComputeNormal(float3 p)
{
    float3 n;
    n.x = MapD(float3(p.x + 0.001f, p.y, p.z)) - MapD(float3(p.x - 0.001f, p.y, p.z));
    n.y = MapD(float3(p.x, p.y + 0.001f, p.z)) - MapD(float3(p.x, p.y - 0.001f, p.z));
    n.z = MapD(float3(p.x, p.y, p.z + 0.001f)) - MapD(float3(p.x, p.y, p.z - 0.001f));
    return normalize(n);
}

[numthreads(8, 8, 1)]
[RootSignature(RootSig)]
void main(uint3 dispatch_tid : SV_DispatchThreadID)
{
    float2 pn = -1.0f + 2.0f * float2(dispatch_tid.x, 1024.0f - dispatch_tid.y) / float2(1024.0f, 1024.0f);

    float3 ro = float3(0.0f, 0.0f, 0.0f);
    float3 rd = normalize(float3(pn.x, pn.y, -1.5f));

    float4 obj = Intersect(ro, rd);
    float3 color = float3(0.0f, 0.0f, 0.0f);

    if (obj.w < kViewDistance)
    {
        float3 p = ro + rd * obj.w;
        float3 n = ComputeNormal(p);
        float3 l = normalize(float3(15.0f, 15.0f, 15.0f) - p);

        float s = Shadow(p + l * 0.01f, l);
        color = max(0.0f, dot(n, l)) * obj.rgb * s;
    }

    s_Target[dispatch_tid.xy] += float4(color, 1.0f);
}
//==============================================================================

#endif
