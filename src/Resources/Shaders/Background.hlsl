struct VS_INPUT
{
    float4 inPosition : POSITION;
    float4 inColor    : COLOR;
    float2 inTexCoord : TEXCOORD;
};

struct VS_OUPUT
{
    float4 outPosition : POSITION;
    float4 outColor    : COLOR;
    float2 outTexCoord : TEXCOORD;
};

VS_OUPUT BackgroundVertex(VS_INPUT input)
{
    VS_OUPUT output;
    output.outPosition = input.inPosition;
    output.outColor    = input.inColor;
    output.outTexCoord = input.inTexCoord;

    return output;
}

struct PS_INPUT
{
    float4 inColor    : COLOR;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture         : register(t0);
SamplerState objSamplerState : register(s0);

float4 BackgroundPixel(PS_INPUT input) : SV_TARGET
{
    float4 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord) * input.inColor;
	return pixelColor;
}
