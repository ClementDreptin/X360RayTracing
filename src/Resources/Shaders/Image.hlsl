#define DISPLAY_WIDTH 1280.0f
#define DISPLAY_HEIGHT 720.0f

float4 c_CameraPosition : register(c0);
float4x4 c_InverseProjection : register(c1);
float4x4 c_InverseView : register(c2);

struct Ray
{
    float3 Origin;
    float3 Direction;
};

float3 CalculateRayDirection(float2 coord)
{
    // float4 target = mul(c_InverseProjection, float4(coord.xy, 1.0f, 1.0f));
    // float4 norm = normalize(target / target.w);

    // return mul(c_InverseView, norm);
    return float3(coord.xy, -1.0f);
}

float4 TraceRay(Ray ray, float4 sphereColor)
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

    float radius = 0.5f;
    float a = dot(ray.Direction, ray.Direction);
    float b = 2.0f * dot(ray.Origin, ray.Direction);
    float c = dot(ray.Origin, ray.Origin) - (radius * radius);

    // Quadratic forumula discriminant:
    // b^2 - 4ac
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
        return float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Quadratic formula solutions:
    // (-b +- sqrt(discriminant)) / 2a
    //
    // The "-" solution is always smaller so it is always the closest hit
    // distance, no need to calculate the "+" solution
    float closestT = (-b - sqrt(discriminant)) / (2.0f * a);
    float3 hitPoint = ray.Origin + ray.Direction * closestT;

    // Light calculation
    float3 normal = normalize(hitPoint);
    float3 lightDir = normalize(float3(-1.0f, -1.0f, -1.0f));
    float lightIntensity = max(dot(normal, -lightDir), 0.0f);

    return sphereColor * lightIntensity;
}

float4 ImagePixel(float2 screenPos : VPOS) : COLOR
{
    float2 coord = float2(screenPos.x / DISPLAY_WIDTH, (DISPLAY_HEIGHT - screenPos.y) / DISPLAY_HEIGHT);
    coord = coord * 2.0f - 1.0f;

    Ray ray;
    ray.Origin = c_CameraPosition;
    ray.Direction = CalculateRayDirection(coord);

    return TraceRay(ray, float4(1.0f, 0.0f, 1.0f, 1.0f));
}

float4 ImageVertex(float4 position : POSITION) : POSITION
{
    return position;
}
