#define DISPLAY_WIDTH 1280.0f
#define DISPLAY_HEIGHT 720.0f

#define MULTIPLIER 0.89f
// Rounded down to match the actual texture size created on the CPU side
#define TEXTURE_WIDTH floor(DISPLAY_WIDTH * MULTIPLIER)
#define TEXTURE_HEIGHT floor(DISPLAY_HEIGHT * MULTIPLIER)

#define UINT_MAX 0xFFFFFFFF
#define FLOAT_MAX 3.402823466e+38F

#define SPHERE_COUNT 3
#define BOUNCES 3

#define RANDOM_TEXTURE_SIZE 256.0f

struct Material
{
    float4 Albedo;
    float4 EmissionColor;
    float EmissionPower;
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

struct Vertex
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

float3 CalculateRayDirection(float2 coord);

HitPayload TraceRay(Ray ray);

HitPayload ClosestHit(Ray ray, float hitDistance, uint objectIndex);

HitPayload Miss(Ray ray);
