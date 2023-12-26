#define DISPLAY_WIDTH 1280.0f
#define DISPLAY_HEIGHT 720.0f

#define UINT_MAX 0xFFFFFFFF
#define FLOAT_MAX 3.402823466e+38F

#define SPHERE_COUNT 2
#define BOUNCES 3

struct Material
{
    float4 Albedo;
};

struct Sphere
{
    float3 Position;
    float Radius;
    Material Material;
};

struct Scene
{
    Sphere Spheres[SPHERE_COUNT];
};

struct Ray
{
    float3 Origin;
    float3 Direction;
};

struct HitPayload
{
    float HitDistance;
    float3 WorldPosition;
    float3 WorldNormal;
    uint ObjectIndex;
};

float3 CalculateRayDirection(float2 coord);

HitPayload TraceRay(Ray ray);

HitPayload ClosestHit(Ray ray, float hitDistance, uint objectIndex);

HitPayload Miss(Ray ray);
