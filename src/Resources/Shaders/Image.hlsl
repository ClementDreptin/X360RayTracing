float4x4 c_matWP : register(c0);

struct VS_INPUT
{
    float4 InPosition : POSITION;
    float2 InTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 OutPosition : POSITION;
    float2 OutTexCoord : TEXCOORD;
};

VS_OUTPUT ImageVertex(VS_INPUT input)
{
    VS_OUTPUT output;
    output.OutPosition = mul(c_matWP, input.InPosition);
    output.OutTexCoord = input.InTexCoord;

    return output;
}

struct PS_INPUT
{
    float2 InTexCoord : TEXCOORD;
};

Texture2D objTexture         : register(t0);
SamplerState objSamplerState : register(s0);

float4 ImagePixel(PS_INPUT input) : SV_TARGET
{
    return objTexture.Sample(objSamplerState, input.InTexCoord);
}
