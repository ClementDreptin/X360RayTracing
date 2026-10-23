#include "Declarations.hlsl"

Vertex TextureVertex(Vertex input)
{
    return input;
}

sampler2D s_AccumulationTextureRG : register(s0);
sampler2D s_AccumulationTextureB : register(s2);

float4 TexturePixel(Vertex input) : COLOR
{
    // The accumulated color is split across 2 textures (red and green in one, blue in the other)
    float2 rg = tex2D(s_AccumulationTextureRG, input.TexCoord).xy;
    float b = tex2D(s_AccumulationTextureB, input.TexCoord).x;

    // The accumulation textures hold unclamped colors, only clamp them when displaying
    return saturate(float4(rg, b, 1.0f));
}
