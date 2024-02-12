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
    float2 TexCoord : TEXCOORD;
};

sampler Sampler : register(s0);

float4 TexturePixel(PS_INPUT input) : SV_TARGET
{
    return tex2D(Sampler, input.TexCoord);
}
