struct VsOut
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;
};

VsOut main(float4 pos : POSITION, float3 color : COLOR)
{
    VsOut vso;
    vso.pos = pos;
    vso.color = color;
    return vso;
}
