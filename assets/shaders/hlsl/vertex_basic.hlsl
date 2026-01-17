struct VSInput
{
    float2 aPos   : POSITION;
    float3 aColor : COLOR0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 vColor   : COLOR0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.aPos, 0.0f, 1.0f);
    output.vColor = input.aColor;
    return output;
}
