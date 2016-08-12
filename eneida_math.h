struct float4
{
    float m_X;
    float m_Y;
    float m_Z;
    float m_W;
};

struct float4x4
{
    float4 m_R[4];
};

// defined in eneida_asmlib.asm
extern "C" float Sin1f(float x);
extern "C" float Cos1f(float x);

inline float4 operator+(const float4& v0, const float4& v1)
{
    float4 res;
    res.m_X = v0.m_X + v1.m_X;
    res.m_Y = v0.m_Y + v1.m_Y;
    res.m_Z = v0.m_Z + v1.m_Z;
    res.m_W = v0.m_W + v1.m_W;
    return res;
}
