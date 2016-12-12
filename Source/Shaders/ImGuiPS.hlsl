#include "ImGuiCommon.hlsli"

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

[RootSignature(RsImgui)]
float4 main(VsOutput i) : SV_Target0
{
    return i.Color * gTexture.Sample(gSampler, i.Texcoord);
}
