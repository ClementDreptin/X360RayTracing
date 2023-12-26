#pragma once

#define SPHERE_COUNT 2

_DECLSPEC_ALIGN_16_ struct Material
{
    Material()
        : Albedo(XMVectorSplatOne())
    {
    }

    XMVECTOR Albedo;
};

_DECLSPEC_ALIGN_16_ struct Sphere
{
    Sphere()
        : Position(XMVectorZero()), Radius(0.5f)
    {
    }

    XMVECTOR Position;
    float Radius;
    Material Material;
};

_DECLSPEC_ALIGN_16_ struct Scene
{
    Sphere Spheres[SPHERE_COUNT];
};
