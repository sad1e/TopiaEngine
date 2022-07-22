#pragma once

#include <Topia.h>

namespace topia
{

/// Class that holds 4 float values. Convert to Vec4 to perform calculations.
class TOPIA_NODISCARD Float4
{
public:
    Float4() = default;
    Float4(const Float4& inRHS) = default;
    Float4(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW) {}

    float operator[](int inCoordinate) const
    {
        ASSERT(inCoordinate < 4);
        return *(&x + inCoordinate);
    }

    float x;
    float y;
    float z;
    float w;
};

static_assert(std::is_trivial<Float4>(), "Is supposed to be a trivial type!");

}