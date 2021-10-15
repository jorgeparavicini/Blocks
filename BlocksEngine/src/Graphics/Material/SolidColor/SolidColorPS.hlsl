cbuffer Color
{
	float4 Color[6]: COLOR;
}

float4 main(const uint tid: SV_PrimitiveID) : SV_TARGET
{
	return Color[tid / 2];
}