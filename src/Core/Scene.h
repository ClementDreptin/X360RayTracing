#pragma once

_DECLSPEC_ALIGN_16_ struct Sphere
{
    Sphere()
        : Position(XMVectorZero()), Albedo(XMVectorSplatOne()), Radius(0.5f)
    {
    }

    XMVECTOR Position;
    XMVECTOR Albedo;
    float Radius;
};

_DECLSPEC_ALIGN_16_ struct Scene
{
    Sphere Spheres[1];
};
