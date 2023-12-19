#include "pch.h"
#include "Renderer/Renderer.h"

#include "Renderer/Globals.h"
#include "Renderer/Ray.h"

#define BOUNCES 3
#define NUM_THREADS (MAXIMUM_PROCESSORS * 2)

namespace Utils
{

static uint32_t pcgHash(uint32_t input)
{
    uint32_t state = input * 747796405 + 2891336453;
    uint32_t word = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;

    return (word >> 22) ^ word;
}

static float RandomFloat(uint32_t &seed)
{
    seed = pcgHash(seed);

    return static_cast<float>(seed) / static_cast<float>(std::numeric_limits<uint32_t>::max());
}

}

Renderer::Renderer()
    : m_FrameIndex(1), m_pActiveScene(nullptr), m_pActiveCamera(nullptr)
{
}

Renderer::~Renderer()
{
    delete[] m_pAccumulationData;
}

HRESULT Renderer::Init()
{
    HRESULT hr = S_OK;

    hr = m_Image.Init();
    if (FAILED(hr))
        Log::Error("Couldn't initialized the image");

    m_pAccumulationData = new XMVECTOR[IMAGE_WIDTH * IMAGE_HEIGHT];

    return hr;
}

void Renderer::Render(const Scene &scene, const Camera &camera)
{
    m_pActiveScene = &scene;
    m_pActiveCamera = &camera;

    // Reset the accumulation data if it gets invalidated
    if (m_FrameIndex == 1)
        ZeroMemory(m_pAccumulationData, IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(XMVECTOR));

    // Dispatch the rendering work accross multiple threads
    HANDLE threadHandles[NUM_THREADS] = {};
    RenderChunkOptions options[NUM_THREADS] = {};
    XMCOLOR *pData = m_Image.Lock();
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        options[i].ThreadIndex = i;
        options[i].pTextureData = pData;
        options[i].This = this;

        // Create a thread but don't start it right away (CREATE_SUSPENDED flag)
        threadHandles[i] = CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(RenderChunk),
            &options[i],
            CREATE_SUSPENDED,
            nullptr
        );

        // Equally dispatch the threads accross all physical processors
        XSetThreadProcessor(threadHandles[i], i % MAXIMUM_PROCESSORS);

        // Resume the thread once it's bound to a processor
        ResumeThread(threadHandles[i]);
    }

    WaitForMultipleObjects(NUM_THREADS, threadHandles, TRUE, INFINITE);
    for (size_t i = 0; i < NUM_THREADS; i++)
        CloseHandle(threadHandles[i]);

    m_FrameIndex++;
    m_Image.Unlock();

    m_Image.Render();
}

uint32_t Renderer::RenderChunk(const RenderChunkOptions *pOptions)
{
    assert(pOptions != nullptr);
    assert(pOptions->pTextureData != nullptr);
    assert(pOptions->This != nullptr);

    // This function renders a chunk of rows of the final image and is ran multiple times,
    // in parallel, on separate threads

    // Calculate the row range to render based on the thread index
    uint32_t firstRow = IMAGE_HEIGHT / NUM_THREADS * pOptions->ThreadIndex;
    uint32_t lastRow = IMAGE_HEIGHT / NUM_THREADS * (pOptions->ThreadIndex + 1);

    for (uint32_t y = firstRow; y < lastRow; y++)
    {
        for (uint32_t x = 0; x < IMAGE_WIDTH; x++)
        {
            Renderer *This = pOptions->This;

            // Get the current pixel color and add it to the color calculated
            // for this pixel during the previous frame
            XMVECTOR color = This->PerPixel(x, y);
            uint32_t index = x + y * IMAGE_WIDTH;
            This->m_pAccumulationData[index] = This->m_pAccumulationData[index] + color;

            // Average the color to create a smooth image (and prevent it from becoming
            // completely white)
            XMVECTOR accumulatedColor = This->m_pAccumulationData[index];
            accumulatedColor = accumulatedColor / static_cast<float>(This->m_FrameIndex);

            // Convert the XMVECTOR color to a XMCOLOR color and store it in the texture
            XMCOLOR _accumulatedColor;
            XMStoreColor(&_accumulatedColor, accumulatedColor);
            pOptions->pTextureData[index] = _accumulatedColor;
        }
    }

    return 0;
}

XMVECTOR Renderer::PerPixel(uint32_t x, uint32_t y)
{
    assert(m_pActiveScene != nullptr);
    assert(m_pActiveCamera != nullptr);

    Ray ray;
    ray.Origin = m_pActiveCamera->GetPosition();
    ray.Direction = m_pActiveCamera->GetRayDirections()[x + y * IMAGE_WIDTH];

    XMVECTOR light = XMVectorZero();
    XMVECTOR contribution = XMVectorSplatOne();
    uint32_t seed = x + y * IMAGE_WIDTH * m_FrameIndex;

    // Make the ray bounce to calculate reflection
    for (size_t i = 0; i < BOUNCES; i++)
    {
        seed += i;

        // See if the ray hit anything, if not, add the skycolor to the ambient light
        HitPayload payload = TraceRay(ray);
        if (payload.HitDistance < 0.0f)
        {
            XMVECTOR skyColor = XMVectorSet(0.2f, 0.3f, 0.5f, 1.0f);
            light += skyColor * contribution;
            break;
        }

        const Sphere &sphere = m_pActiveScene->Spheres[payload.ObjectIndex];
        const Material &material = m_pActiveScene->Materials[sphere.MaterialIndex];

        // Make the spheres absorb their albedo every time a ray hits them.
        // contribution represents the energy in the light ray and decreases
        // every time a sphere is hit.
        contribution *= material.Albedo;

        // Make the spheres emit light (it only happens if they have a material with
        // some emission power)
        light += material.GetEmission();

        // Once the ray hit the sphere, update its origin to be the hit point
        // right in front of the sphere along the normal. We don't make it exactly
        // the hit position because the hit position might be a little bit inside
        // the sphere due to floating point precision. Which would cause the ray to
        // hit the same sphere again but from the inside.
        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
        ray.Direction =
            payload.WorldNormal +
            XMVector3NormalizeEst(
                XMVectorSet(
                    Utils::RandomFloat(seed) * 2.0f - 1.0f,
                    Utils::RandomFloat(seed) * 2.0f - 1.0f,
                    Utils::RandomFloat(seed) * 2.0f - 1.0f,
                    1.0f
                )
            );
    }

    return light;
}

Renderer::HitPayload Renderer::TraceRay(const Ray &ray)
{
    assert(m_pActiveScene != nullptr);

    // Find the closest sphere along the ray
    uint32_t closestSphereIndex = std::numeric_limits<uint32_t>::max();
    float hitDistance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < m_pActiveScene->Spheres.size(); i++)
    {
        // Math explaination:
        // https://www.youtube.com/watch?v=4NshnkzOdI0
        //
        // Equation of a sphere (which is a quadratic equation):
        // (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 - r^2) = 0
        // (         a        )t^2 + (          b          )t + (            c           ) = 0
        //
        // where
        // a = ray origin
        // b = ray direction
        // r = radius
        // t = hit distance

        const Sphere &sphere = m_pActiveScene->Spheres[i];
        XMVECTOR origin = ray.Origin - sphere.Position;
        float a = XMVector3Dot(ray.Direction, ray.Direction).x;
        float b = 2.0f * XMVector3Dot(origin, ray.Direction).x;
        float c = XMVector3Dot(origin, origin).x - sphere.Radius * sphere.Radius;

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
        float closestT = (-b - sqrtf(discriminant)) / (2.0f * a);
        if (closestT > 0.0f && closestT < hitDistance)
        {
            hitDistance = closestT;
            closestSphereIndex = i;
        }
    }

    // No sphere was hit
    if (closestSphereIndex == std::numeric_limits<uint32_t>::max())
        return Miss(ray);

    return ClosestHit(ray, hitDistance, closestSphereIndex);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray &ray, float hitDistance, uint32_t objectIndex)
{
    assert(m_pActiveScene != nullptr);

    const Sphere &closestSphere = m_pActiveScene->Spheres[objectIndex];

    HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;
    payload.WorldPosition = ray.Origin + ray.Direction * hitDistance;
    payload.WorldNormal = XMVector3NormalizeEst(payload.WorldPosition - closestSphere.Position);

    return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray &ray)
{
    HitPayload payload;
    payload.HitDistance = -1.0f;

    return payload;
}
