#include "Declarations.hlsl"

Vertex TextureVertex(Vertex input)
{
    return input;
}

sampler2D s_Accumulation : register(s0);

float4 TexturePixel(Vertex input) : SV_TARGET
{
    return tex2D(s_Accumulation, input.TexCoord);
}
