#pragma once

#include "Core/Scene.h"
#include "Renderer/Camera.h"
#include "Renderer/Image.h"
#include "Renderer/Ray.h"

#define NUM_THREADS (MAXIMUM_PROCESSORS * 2)

class Renderer
{
public:
    Renderer();

    ~Renderer();

    HRESULT Init();

    void Render(const Scene &scene, const Camera &camera);

    void ResetAccumulation() { m_FrameIndex = 1; }

private:
    Image m_Image;
    XMVECTOR *m_pAccumulationData;
    uint32_t m_FrameIndex;
    const Scene *m_pActiveScene;
    const Camera *m_pActiveCamera;

    // There is one random engine per thread to avoid CPU cache misses when
    // the engine reads from its internal state.
    static std::default_random_engine s_RandEngine[NUM_THREADS];
    static std::uniform_real_distribution<float> s_Rand;

    struct RenderChunkOptions
    {
        RenderChunkOptions()
            : ThreadIndex(std::numeric_limits<uint32_t>::max()), pTextureData(nullptr), This(nullptr)
        {
        }

        uint32_t ThreadIndex;
        XMCOLOR *pTextureData;
        Renderer *This;
    };

    struct HitPayload
    {
        HitPayload()
            : HitDistance(0.0f), WorldPosition(XMVectorZero()),
              WorldNormal(XMVectorZero()), ObjectIndex(std::numeric_limits<uint32_t>::max())
        {
        }

        float HitDistance;
        XMVECTOR WorldPosition;
        XMVECTOR WorldNormal;
        uint32_t ObjectIndex;
    };

    static uint32_t RenderChunk(const RenderChunkOptions *pOptions);

    XMVECTOR PerPixel(uint32_t x, uint32_t y, uint32_t threadIndex);

    HitPayload TraceRay(const Ray &ray);

    HitPayload ClosestHit(const Ray &ray, float hitDistance, uint32_t objectIndex);

    HitPayload Miss(const Ray &ray);
};
