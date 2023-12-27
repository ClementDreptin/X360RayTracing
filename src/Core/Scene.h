#pragma once

#define SPHERE_COUNT 3

_DECLSPEC_ALIGN_16_ struct Material
{
    Material()
        : Albedo(XMVectorSplatOne()), Roughness(1.0f), EmissionColor(XMVectorZero()), EmissionPower(0.0f)
    {
    }

    XMVECTOR Albedo;
    XMVECTOR EmissionColor;
    float Roughness;
    float _padding1[3];
    float EmissionPower;
    float _padding2[3];
};

_DECLSPEC_ALIGN_16_ struct Sphere
{
    Sphere()
        : Position(XMVectorZero()), Radius(0.5f)
    {
    }

    XMVECTOR Position;
    float Radius;
    float _padding1[3];
    Material Material;
};

_DECLSPEC_ALIGN_16_ struct Scene
{
    Sphere Spheres[SPHERE_COUNT];
};
