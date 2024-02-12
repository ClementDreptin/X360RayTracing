struct Vertex
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

Vertex TextureVertex(Vertex input)
{
    return input;
}

sampler Sampler : register(s0);

float4 TexturePixel(Vertex input) : SV_TARGET
{
    return tex2D(Sampler, input.TexCoord);
}
