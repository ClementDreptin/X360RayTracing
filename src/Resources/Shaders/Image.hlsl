#include "Declarations.hlsl"

float c_FrameIndex : register(c0);
float4 c_CameraPosition : register(c1);
float4x4 c_InverseProjection : register(c4);
float4x4 c_InverseView : register(c8);
Scene c_Scene : register(c12);

sampler2D s_AccumulationTexture : register(s0);

float3 CalculateRayDirection(float2 coord)
{
    // Convert the pixel coordinates to world space coordinates based
    // on where the camera is looking

    float4 target = mul(c_InverseProjection, float4(coord, 1.0f, 1.0f));
    float4 norm = normalize(target / target.w);

    return mul(c_InverseView, norm);
}

HitPayload TraceRay(Ray ray)
{
    // Find the closest sphere along the ray
    uint closestSphereIndex = UINT_MAX;
    float hitDistance = FLOAT_MAX;
    for (uint i = 0; i < SPHERE_COUNT; i++)
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
        return Miss(ray);

    return ClosestHit(ray, hitDistance, closestSphereIndex);
}

HitPayload ClosestHit(Ray ray, float hitDistance, uint objectIndex)
{
    Sphere sphere = c_Scene.Spheres[objectIndex];

    HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;
    payload.WorldPosition = ray.Origin + ray.Direction * hitDistance;
    payload.WorldNormal = normalize(payload.WorldPosition - sphere.Position);

    return payload;
}

HitPayload Miss(Ray ray)
{
    HitPayload payload;
    payload.HitDistance = -1.0f;
    payload.ObjectIndex = UINT_MAX;
    payload.WorldPosition = float3(0.0f, 0.0f, 0.0f);
    payload.WorldNormal = float3(0.0f, 0.0f, 0.0f);

    return payload;
}

// Pixel shader entry point
float4 ImagePixel(Vertex input, float2 screenPos : VPOS) : COLOR
{
    // Normalize the screen coordinates and convert them to a [-1;+1] range
    float2 coord = float2(screenPos.x / TEXTURE_WIDTH, (TEXTURE_HEIGHT - screenPos.y) / TEXTURE_HEIGHT);
    coord = coord * 2.0f - 1.0f;

    // Create a ray that starts at the camera and goes where it's looking
    Ray ray;
    ray.Origin = c_CameraPosition;
    ray.Direction = CalculateRayDirection(coord);

    float3 light = float3(0.0f, 0.0f, 0.0f);
    float3 contribution = float3(1.0f, 1.0f, 1.0f);
    for (uint i = 0; i < BOUNCES; i++)
    {
        // See if the ray hit aything, if not, return the skycolor
        HitPayload payload = TraceRay(ray);
        if (payload.HitDistance < 0.0f)
        {
            float3 skyColor = float3(0.2f, 0.3f, 0.5f);
            light += skyColor * contribution;
            break;
        }

        Sphere sphere = c_Scene.Spheres[payload.ObjectIndex];

        // Make the spheres absorb their albedo every time a ray hits them.
        // contribution represents the energy in the light ray and decreases
        // every time a sphere is hit.
        contribution *= sphere.Material.Albedo;

        // Make the spheres emit light (it only happens if they have a material with
        // some emission power)
        light += sphere.Material.EmissionColor * sphere.Material.EmissionPower;

        // Once the ray hit the sphere, update its origin to be the hit point
        // right in front of the sphere along the normal. We don't make it exactly
        // the hit position because the hit position might be a little bit inside
        // the sphere due to floating point precision. Which would cause the ray to
        // hit the same sphere again but from the inside.
        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;

        // Reflect along the normal with a random offset
        ray.Direction =
            payload.WorldNormal +
            normalize(float3(
                random(coord * c_FrameIndex + 1) * 2.0f - 1.0f,
                random(coord * c_FrameIndex + 2) * 2.0f - 1.0f,
                random(coord * c_FrameIndex + 3) * 2.0f - 1.0f
            ));
    }

    float4 accumulatedColor = tex2D(s_AccumulationTexture, input.TexCoord);
    float4 currentColor = float4(light, 1.0f);

    return (accumulatedColor * (c_FrameIndex - 1.0f) + currentColor) / c_FrameIndex;
}

// Vertex shader entry point
Vertex ImageVertex(Vertex input)
{
    return input;
}
