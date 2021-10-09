cbuffer CBuf
{
    matrix Transform;
}

float4 main(const float3 pos : POSITION) : SV_POSITION
{
    return mul(float4(pos, 1), Transform);
}
