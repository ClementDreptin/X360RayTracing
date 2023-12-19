#pragma once

struct Material
{
    Material()
        : Albedo(XMVectorSplatOne()), EmissionColor(XMVectorZero()), EmissionPower(0.0f)
    {
    }

    XMVECTOR Albedo;
    XMVECTOR EmissionColor;
    float EmissionPower;

    XMVECTOR GetEmission() const { return EmissionColor * EmissionPower; }
};

struct Sphere
{
    Sphere()
        : Position(XMVectorZero()), Radius(0.5f), MaterialIndex(std::numeric_limits<uint32_t>::max())
    {
    }

    XMVECTOR Position;
    float Radius;
    uint32_t MaterialIndex;
};

struct Scene
{
    std::vector<Sphere> Spheres;
    std::vector<Material> Materials;
};
