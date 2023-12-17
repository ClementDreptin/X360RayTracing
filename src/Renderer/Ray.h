#pragma once

struct Ray
{
    Ray()
        : Origin(XMVectorZero()), Direction(XMVectorZero())
    {
    }

    XMVECTOR Origin;
    XMVECTOR Direction;
};
