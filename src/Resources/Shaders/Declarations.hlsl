#define DISPLAY_WIDTH 1280.0f
#define DISPLAY_HEIGHT 720.0f
#define UINT_MAX 0xFFFFFFFF
#define FLOAT_MAX 3.402823466e+38F
#define SPHERE_COUNT 2

struct Sphere
{
    float3 Position;
    float4 Albedo;
    float Radius;
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

float3 CalculateRayDirection(float2 coord);

float4 TraceRay(Ray ray);
