struct PSInput
{
    float3 vColor : COLOR0;
};

float4 main(PSInput input) : SV_TARGET
{
    return float4(input.vColor, 1.0f);
}
