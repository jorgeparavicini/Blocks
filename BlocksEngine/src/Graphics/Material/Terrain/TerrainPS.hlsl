Texture2DArray Textures;
SamplerState Splr;

float4 main(const float2 texCoord : TEXCOORD, const uint textureIndex : TEXINDEX) : SV_TARGET
{
	return Textures.Sample(Splr, float3(texCoord[0], texCoord[1], textureIndex));
}