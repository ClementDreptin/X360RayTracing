#define DISPLAY_WIDTH 1280.0f
#define DISPLAY_HEIGHT 720.0f

float4 ImageVertex(float4 position : POSITION) : POSITION
{
    return position;
}

float4 ImagePixel(float2 screenPos : VPOS) : COLOR
{
    return float4(screenPos.x / DISPLAY_WIDTH, screenPos.y / DISPLAY_HEIGHT, 0.0f, 1.0f);
}
