#pragma once

struct Sphere
{
    Sphere()
        : Position(XMVectorZero()), Radius(0.5f), Albedo(XMVectorSplatOne())
    {
    }

    XMVECTOR Position;
    float Radius;
    XMVECTOR Albedo;
};

struct Scene
{
    std::vector<Sphere> Spheres;
};
