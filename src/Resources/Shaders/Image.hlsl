float4x4 c_matWP : register(c0);

struct VS_INPUT
{
    float4 InPosition : POSITION;
    float4 InColor    : COLOR;
    float2 InTexCoord : TEXCOORD;
};

struct VS_OUPUT
{
    float4 OutPosition : POSITION;
    float4 OutColor    : COLOR;
    float2 OutTexCoord : TEXCOORD;
};

VS_OUPUT ImageVertex(VS_INPUT input)
{
    VS_OUPUT output;
    output.OutPosition = mul(c_matWP, input.InPosition);
    output.OutColor    = input.InColor;
    output.OutTexCoord = input.InTexCoord;

    return output;
}

struct PS_INPUT
{
    float4 InColor    : COLOR;
    float2 InTexCoord : TEXCOORD;
};

Texture2D objTexture         : register(t0);
SamplerState objSamplerState : register(s0);

float4 ImagePixel(PS_INPUT input) : SV_TARGET
{
    return objTexture.Sample(objSamplerState, input.InTexCoord) * input.InColor;
}
