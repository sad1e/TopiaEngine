#pragma once

#include <Topia.h>

namespace topia
{
/// Class that holds 3 floats. Used as a storage class. Convert to Vec3 for calculations.
class TOPIA_NODISCARD Float3
{
public:
    Float3() = default;
    Float3(const Float3& inRHS) = default;
    Float3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

    float operator[](int inCoordinate) const
    {
        ASSERT(inCoordinate < 3);
        return *(&x + inCoordinate);
    }

    bool operator==(const Float3& inRHS) const
    {
        return x == inRHS.x && y == inRHS.y && z == inRHS.z;
    }

    bool operator!=(const Float3& inRHS) const
    {
        return x != inRHS.x || y != inRHS.y || z != inRHS.z;
    }

    float x;
    float y;
    float z;
};

using VertexList = std::vector<Float3>;

static_assert(std::is_trivial<Float3>(), "Is supposed to be a trivial type!");
}

// Create a std::hash for Float3
TOPIA_MAKE_HASHABLE(topia::Float3, t.x, t.y, t.z)
