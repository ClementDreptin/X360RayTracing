#include "Declarations.hlsl"

Vertex TextureVertex(Vertex input)
{
    return input;
}

sampler2D s_AccumulationTexture : register(s0);

float4 TexturePixel(Vertex input) : COLOR
{
    return tex2D(s_AccumulationTexture, input.TexCoord);
}
