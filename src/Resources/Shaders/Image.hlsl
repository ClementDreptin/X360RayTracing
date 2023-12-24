#include "Declarations.hlsl"

uniform float4 c_CameraPosition : register(c0);
uniform float4x4 c_InverseProjection : register(c4);
uniform float4x4 c_InverseView : register(c8);

cbuffer CB_SCENE : register(c12)
{
    Scene c_Scene;
};

float3 CalculateRayDirection(float2 coord)
{
    // Convert the pixel coordinates to world space coordinates based
    // on where the camera is looking

    float4 target = mul(c_InverseProjection, float4(coord.xy, 1.0f, 1.0f));
    float4 norm = normalize(target / target.w);

    return mul(c_InverseView, norm);
}

float4 TraceRay(Ray ray)
{
    // Find the closest sphere along the ray
    uint closestSphereIndex = UINT_MAX;
    float hitDistance = FLOAT_MAX;
    for (int i = 0; i < SPHERE_COUNT; i++)
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

        Sphere sphere = c_Scene.Spheres[i];
        float3 origin = ray.Origin - sphere.Position;
        float a = dot(ray.Direction, ray.Direction);
        float b = 2.0f * dot(origin, ray.Direction);
        float c = dot(origin, origin) - (sphere.Radius * sphere.Radius);

        // Quadratic forumula discriminant:
        // b^2 - 4ac
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f)
            continue;

        // Quadratic formula solutions:
        // (-b +- sqrt(discriminant)) / 2a
        //
        // The "-" solution is always smaller so it is always the closest hit
        // distance, no need to calculate the "+" solution
        float closestT = (-b - sqrt(discriminant)) / (2.0f * a);
        if (closestT > 0.0f && closestT < hitDistance)
        {
            hitDistance = closestT;
            closestSphereIndex = i;
        }
    }

    // No spheres were hit
    if (closestSphereIndex == UINT_MAX)
        return float4(0.0f, 0.0f, 0.0f, 1.0f);

    Sphere closestSphere = c_Scene.Spheres[closestSphereIndex];
    float3 origin = ray.Origin - closestSphere.Position.xyz;
    float3 hitPoint = origin + ray.Direction * hitDistance;

    // Light calculation
    float3 normal = normalize(hitPoint);
    float3 lightDir = normalize(float3(-1.0f, -1.0f, -1.0f));
    float lightIntensity = max(dot(normal, -lightDir), 0.0f);

    return closestSphere.Albedo * lightIntensity;
}

// Pixel shader entry point
float4 ImagePixel(float2 screenPos : VPOS) : COLOR
{
    // Normalize the screen coordinates and convert them to a [-1;+1] range
    float2 coord = float2(screenPos.x / DISPLAY_WIDTH, (DISPLAY_HEIGHT - screenPos.y) / DISPLAY_HEIGHT);
    coord = coord * 2.0f - 1.0f;

    // Create a ray that starts at the camera and goes where the camera is looking
    Ray ray;
    ray.Origin = c_CameraPosition;
    ray.Direction = CalculateRayDirection(coord);

    return TraceRay(ray);
}

// Vertex shader entry point
float4 ImageVertex(float4 position : POSITION) : POSITION
{
    return position;
}
