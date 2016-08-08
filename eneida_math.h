struct float4
{
    float X, Y, Z, W;
};

struct float4x4
{
    float4 R[4];
};

inline float4
operator+(const float4 &V0, const float4 &V1)
{
    float4 Res;
    Res.X = V0.X + V1.X;
    Res.Y = V0.Y + V1.Y;
    Res.Z = V0.Z + V1.Z;
    Res.W = V0.W + V1.W;
    return Res;
}

inline float4
operator-(const float4 &V0, const float4 &V1)
{
    float4 Res;
    Res.X = V0.X - V1.X;
    Res.Y = V0.Y - V1.Y;
    Res.Z = V0.Z - V1.Z;
    Res.W = V0.W - V1.W;
    return Res;
}

inline float4
operator*(const float4 &V0, const float4 &V1)
{
    float4 Res;
    Res.X = V0.X * V1.X;
    Res.Y = V0.Y * V1.Y;
    Res.Z = V0.Z * V1.Z;
    Res.W = V0.W * V1.W;
    return Res;
}

inline float4
operator/(const float4 &V0, const float4 &V1)
{
    float4 Res;
    Res.X = V0.X / V1.X;
    Res.Y = V0.Y / V1.Y;
    Res.Z = V0.Z / V1.Z;
    Res.W = V0.W / V1.W;
    return Res;
}

inline float4x4
operator+(const float4x4 &M0, const float4x4 &M1)
{
    float4x4 Res;
    Res.R[0] = M0.R[0] + M1.R[0];
    Res.R[1] = M0.R[1] + M1.R[1];
    Res.R[2] = M0.R[2] + M1.R[2];
    Res.R[3] = M0.R[3] + M1.R[3];
    return Res;
}

inline float4x4
operator-(const float4x4 &M0, const float4x4 &M1)
{
    float4x4 Res;
    Res.R[0] = M0.R[0] - M1.R[0];
    Res.R[1] = M0.R[1] - M1.R[1];
    Res.R[2] = M0.R[2] - M1.R[2];
    Res.R[3] = M0.R[3] - M1.R[3];
    return Res;
}

inline float4x4
operator*(const float4x4 &M0, const float4x4 &M1)
{
    float4x4 Res;
    Res.R[0] = M0.R[0] * M1.R[0];
    Res.R[1] = M0.R[1] * M1.R[1];
    Res.R[2] = M0.R[2] * M1.R[2];
    Res.R[3] = M0.R[3] * M1.R[3];
    return Res;
}

inline float4x4
operator/(const float4x4 &M0, const float4x4 &M1)
{
    float4x4 Res;
    Res.R[0] = M0.R[0] / M1.R[0];
    Res.R[1] = M0.R[1] / M1.R[1];
    Res.R[2] = M0.R[2] / M1.R[2];
    Res.R[3] = M0.R[3] / M1.R[3];
    return Res;
}
