cbuffer CBuf
{
matrix Wvp;
}

struct VsOut
{
    float2 tex : TEXCOORD;
    uint textureIndex : TEXINDEX;
    float4 pos : SV_POSITION;
};

VsOut main(const float3 pos : POSITION, const float2 tex : TEXCOORD, const uint textureIndex : TEXINDEX)
{
    VsOut vso;
    vso.pos = mul(float4(pos, 1), Wvp);
    vso.tex = tex;
    vso.textureIndex = textureIndex;
    return vso;
}
