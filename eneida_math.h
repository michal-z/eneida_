typedef __m128 float4;

__declspec(align(16))
struct float4x4
{
    float4 R[4];
};

__forceinline float4 __vectorcall
operator+(float4 V0, float4 V1)
{
    return _mm_add_ps(V0, V1);
}

__forceinline float4 __vectorcall
operator-(float4 V0, float4 V1)
{
    return _mm_sub_ps(V0, V1);
}

__forceinline float4 __vectorcall
operator*(float4 V0, float4 V1)
{
    return _mm_mul_ps(V0, V1);
}

__forceinline float4 __vectorcall
operator/(float4 V0, float4 V1)
{
    // TODO: add assertion here to ensure that V1 != 0
    return _mm_div_ps(V0, V1);
}

__forceinline float4x4 __vectorcall
operator+(float4x4 M0, const float4x4 &M1)
{
    float4x4 Res;
    Res.R[0] = _mm_add_ps(M0.R[0], M1.R[0]);
    Res.R[1] = _mm_add_ps(M0.R[1], M1.R[1]);
    Res.R[2] = _mm_add_ps(M0.R[2], M1.R[2]);
    Res.R[3] = _mm_add_ps(M0.R[3], M1.R[3]);
    return Res;
}

__forceinline float4x4 __vectorcall
operator-(float4x4 M0, const float4x4 &M1)
{
    float4x4 Res;
    Res.R[0] = _mm_sub_ps(M0.R[0], M1.R[0]);
    Res.R[1] = _mm_sub_ps(M0.R[1], M1.R[1]);
    Res.R[2] = _mm_sub_ps(M0.R[2], M1.R[2]);
    Res.R[3] = _mm_sub_ps(M0.R[3], M1.R[3]);
    return Res;
}

__forceinline float4x4 __vectorcall
operator*(float4x4 M0, const float4x4 &M1)
{
    float4x4 Res;
    Res.R[0] = _mm_mul_ps(M0.R[0], M1.R[0]);
    Res.R[1] = _mm_mul_ps(M0.R[1], M1.R[1]);
    Res.R[2] = _mm_mul_ps(M0.R[2], M1.R[2]);
    Res.R[3] = _mm_mul_ps(M0.R[3], M1.R[3]);
    return Res;
}

__forceinline float4x4 __vectorcall
operator/(float4x4 M0, const float4x4 &M1)
{
    // TODO: add assertions
    float4x4 Res;
    Res.R[0] = _mm_div_ps(M0.R[0], M1.R[0]);
    Res.R[1] = _mm_div_ps(M0.R[1], M1.R[1]);
    Res.R[2] = _mm_div_ps(M0.R[2], M1.R[2]);
    Res.R[3] = _mm_div_ps(M0.R[3], M1.R[3]);
    return Res;
}
