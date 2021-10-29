cbuffer Color
{
	float4 Color[6]: COLOR;
}

float4 main(const uint tid: SV_PrimitiveID) : SV_TARGET
{
	if (tid < 6)
	{
		return float4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	return float4(((tid / 12) * 9999 % 255) / 255.0f, ((tid / 12) * 99999 % 255) / 255.0f, ((tid / 12) * 999999 % 255) / 255.0f, 1.0f);
	//return Color[(tid % 6) / 2];
}