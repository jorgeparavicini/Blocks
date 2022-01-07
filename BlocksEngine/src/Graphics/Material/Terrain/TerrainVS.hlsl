cbuffer CBuf : register(b0)
{
matrix WVP;
matrix World;
}

struct VsOutput
{
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    uint textureIndex : TEXINDEX;
    float4 pos : SV_POSITION;
};

VsOutput main(const float3 pos : POSITION, const float2 tex : TEXCOORD, const float3 normal : NORMAL,
              const uint textureIndex : TEXINDEX)
{
    VsOutput vso;
    vso.pos = mul(float4(pos, 1), WVP);
    vso.normal = normal;
    vso.tex = tex;
    vso.textureIndex = textureIndex;
    return vso;
}
