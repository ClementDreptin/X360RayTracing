struct Vertex
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

Vertex TextureVertex(Vertex input)
{
    return input;
}

float c_FrameIndex : register(c0);
sampler2D s_Accumulation : register(s0);

float4 TexturePixel(Vertex input) : SV_TARGET
{
    return float4(tex2D(s_Accumulation, input.TexCoord).rgb / c_FrameIndex, 1.0f);
}
