struct Light
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
};

cbuffer cbPerFrame
{
Light light;
}

Texture2DArray Textures;
SamplerState Sampler;

struct PsInput
{
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    uint textureIndex : TEXINDEX;
    float4 pos : SV_POSITION;
};

float4 main(PsInput input) : SV_TARGET
{
    input.normal = normalize(input.normal);

    const float4 diffuse = Textures.Sample(Sampler, float3(input.tex[0], input.tex[1], input.textureIndex));

    float3 finalColor = diffuse * light.ambient;
    finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);

    return float4(finalColor, diffuse.a);
}
