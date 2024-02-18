struct Vertex
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

Vertex AccumulationVertex(Vertex input)
{
    return input;
}

float c_FrameIndex : register(c0);
sampler2D s_AccumulationTexture : register(s0);
sampler2D s_ImageTexture : register(s1);

float4 AccumulationPixel(Vertex input) : SV_TARGET
{
    float4 accumulationColor = tex2D(s_AccumulationTexture, input.TexCoord);
    float4 imageColor = tex2D(s_ImageTexture, input.TexCoord);

    return (accumulationColor * (c_FrameIndex - 1.0f) + imageColor) / c_FrameIndex;
}
