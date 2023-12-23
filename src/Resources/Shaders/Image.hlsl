#define DISPLAY_WIDTH 1280.0f
#define DISPLAY_HEIGHT 720.0f

float4 PerPixel(float2 coord)
{
    // Math explaination:
    // https://www.youtube.com/watch?v=4NshnkzOdI0
    //
    // Equation of a sphere (which is a quadratic equation):
    // (Bx^2 + By^2 + Bz^2)t^2 + (2(AxBx + AyBy + AzBz))t + (Ax^2 + Ay^2 + Az^2 - r^2) = 0
    // (         a        )t^2 + (          b          )t + (            c           ) = 0
    //
    // where
    // A = ray origin
    // B = ray direction
    // r = radius
    // t = hit distance
    // note: "Ax" is the x component of the A vector, not the A vector multipled by some x

    float3 rayOrigin = float3(0.0f, 0.0f, 2.0f);
    float3 rayDirection = float3(coord.x, coord.y, -1.0f);
    float radius = 0.5f;
    float a = dot(rayDirection, rayDirection);
    float b = 2.0f * dot(rayOrigin, rayDirection);
    float c = dot(rayOrigin, rayOrigin) - (radius * radius);

    // Quadratic forumula discriminant:
    // b^2 - 4ac
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant >= 0.0f)
        return float4(1.0f, 0.0f, 1.0f, 1.0f);

    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

float4 ImagePixel(float2 screenPos : VPOS) : COLOR
{
    float2 coord = float2(screenPos.x / DISPLAY_WIDTH, screenPos.y / DISPLAY_HEIGHT);
    coord = coord * 2.0f - 1.0f;

    return PerPixel(coord);
}

float4 ImageVertex(float4 position : POSITION) : POSITION
{
    return position;
}
