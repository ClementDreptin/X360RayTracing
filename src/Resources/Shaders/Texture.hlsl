struct Vertex
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

Vertex TextureVertex(Vertex input)
{
    return input;
}

struct PS_INPUT
{
    float2 InTexCoord : TEXCOORD;
};

Texture2D objTexture         : register(t0);
SamplerState objSamplerState : register(s0);

float4 TexturePixel(PS_INPUT input) : SV_TARGET
{
    return objTexture.Sample(objSamplerState, input.InTexCoord);
}
