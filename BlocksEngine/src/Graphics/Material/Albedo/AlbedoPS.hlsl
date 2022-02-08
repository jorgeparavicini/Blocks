Texture2D Tex;
SamplerState Splr;

float4 main(const float2 texCoord: TEXCOORD) : SV_TARGET
{
	return Tex.Sample(Splr, texCoord);
}