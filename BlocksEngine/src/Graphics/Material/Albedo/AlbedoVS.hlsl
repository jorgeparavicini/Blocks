cbuffer CBuf
{
matrix Wvp;
float4 Color;
}

struct VsOut
{
    float2 tex : TEXCOORD;
    float4 pos : SV_POSITION;
};


VsOut main(const float3 pos : POSITION, const float2 tex : TEXCOORD)
{
    VsOut vso;
    vso.pos = mul(float4(pos, 1), Wvp);
    vso.tex = tex;
    return vso;
}
