#pragma once

#include "Core/Scene.h"
#include "Renderer/Camera.h"
#include "Renderer/Image.h"
#include "Renderer/Ray.h"

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

    static std::default_random_engine s_RandEngine;
    static std::uniform_real_distribution<float> s_Rand;

    struct DoWorkOptions
    {
        uint32_t FirstLine;
        uint32_t LastLine;
        XMCOLOR *pData;
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

    static uint32_t WINAPI DoWork(DoWorkOptions *pOptions);

    XMVECTOR PerPixel(uint32_t x, uint32_t y);

    HitPayload TraceRay(const Ray &ray);

    HitPayload ClosestHit(const Ray &ray, float hitDistance, uint32_t objectIndex);

    HitPayload Miss(const Ray &ray);
};
