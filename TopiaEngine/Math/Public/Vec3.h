#pragma once

#include "Float3.h"
#include "Swizzle.h"
#include "MathTypes.h"

namespace topia
{
/// 3 component vector (stored as 4 vectors).
class TOPIA_NODISCARD Vec3
{
public:
    // Underlying vector type
#if defined(TOPIA_USE_SSE)
    using Type = __m128;
#elif defined(TOPIA_USE_NEON)
    using Type = float32x4_t;
#else
    #error Unsupported platform.
#endif

    Vec3() = default;
    Vec3(const Vec3& inRHS) = default;
    explicit TOPIA_INLINE Vec3(Vec4Arg inRHS);
    TOPIA_INLINE Vec3(Type inRHS) : mValue(inRHS) { CheckW(); }

    /// Load 3 floats from memory
    explicit TOPIA_INLINE Vec3(const Float3& inV);

    /// Create a vector from 3 components
    TOPIA_INLINE Vec3(float inX, float inY, float inZ);

    /// Vector with all zeros
    static TOPIA_INLINE Vec3 sZero();

    /// Vector with all NaN's
    static TOPIA_INLINE Vec3 sNaN();

    /// Vectors with the principal axis
    static TOPIA_INLINE Vec3 sAxisX()    { return Vec3(1, 0, 0);  }
    static TOPIA_INLINE Vec3 sNegAxisX() { return Vec3(-1, 0, 0); }
    static TOPIA_INLINE Vec3 sAxisY()    { return Vec3(0, 1, 0);  }
    static TOPIA_INLINE Vec3 sNegAxisY() { return Vec3(0, -1, 0); }
    static TOPIA_INLINE Vec3 sAxisZ()    { return Vec3(0, 0, 1);  }
    static TOPIA_INLINE Vec3 sNegAxisZ() { return Vec3(0, 0, -1); }

    static TOPIA_INLINE Vec3 sForward()  { return Vec3(0, 0, 1);  }
    static TOPIA_INLINE Vec3 sBack()     { return Vec3(0, 0, -1); }
    static TOPIA_INLINE Vec3 sLeft()     { return Vec3(-1, 0, 0); }
    static TOPIA_INLINE Vec3 sRight()    { return Vec3(1, 0, 0);  }
    static TOPIA_INLINE Vec3 sUp()       { return Vec3(0, 1, 0);  }
    static TOPIA_INLINE Vec3 sDown()     { return Vec3(0, -1, 0); }

    static TOPIA_INLINE Vec3 sOne()      { return Vec3(1, 1, 1);  }

    /// Replicate inV across all components
    static TOPIA_INLINE Vec3 sReplicate(float inV);

    /// Load 3 floats from memory (reads 32 bits extra which it doesn't use)
    static TOPIA_INLINE Vec3 sLoadFloat3Unsafe(const Float3& inV);

    /// Return the minimum value of each of the components
    static TOPIA_INLINE Vec3 sMin(Vec3Arg inV1, Vec3Arg inV2);

    /// Return the maximum of each of the components
    static TOPIA_INLINE Vec3 sMax(Vec3Arg inV1, Vec3Arg inV2);

    /// Clamp a vector between min and max (component wise)
    static TOPIA_INLINE Vec3 sClamp(Vec3Arg inV, Vec3Arg inMin, Vec3Arg inMax);

    /// Equals (component wise)
    static TOPIA_INLINE UVec4 sEquals(Vec3Arg inV1, Vec3Arg inV2);

    /// Less than (component wise)
    static TOPIA_INLINE UVec4 sLess(Vec3Arg inV1, Vec3Arg inV2);

    /// Less than or equal (component wise)
    static TOPIA_INLINE UVec4 sLessOrEqual(Vec3Arg inV1, Vec3Arg inV2);

    /// Greater than (component wise)
    static TOPIA_INLINE UVec4 sGreater(Vec3Arg inV1, Vec3Arg inV2);

    /// Greater than or equal (component wise)
    static TOPIA_INLINE UVec4 sGreaterOrEqual(Vec3Arg inV1, Vec3Arg inV2);

    /// Calculates inMul1 * inMul2 + inAdd
    static TOPIA_INLINE Vec3 sFusedMultiplyAdd(Vec3Arg inMul1, Vec3Arg inMul2, Vec3Arg inAdd);

    /// Component wise select, returns inV1 when highest bit of inControl = 0 and inV2 when highest bit of inControl = 1
    static TOPIA_INLINE Vec3 sSelect(Vec3Arg inV1, Vec3Arg inV2, UVec4Arg inControl);

    /// Logical or (component wise)
    static TOPIA_INLINE Vec3 sOr(Vec3Arg inV1, Vec3Arg inV2);

    /// Logical xor (component wise)
    static TOPIA_INLINE Vec3 sXor(Vec3Arg inV1, Vec3Arg inV2);

    /// Logical and (component wise)
    static TOPIA_INLINE Vec3 sAnd(Vec3Arg inV1, Vec3Arg inV2);

    /// Get unit vector given spherical coordinates
    /// inTheta \f$\in [0, \pi]\f$ is angle between vector and z-axis
    /// inPhi \f$\in [0, 2 \pi]\f$ is the angle in the xy-plane starting from the x axis and rotating counter clockwise around the z-axis
    static TOPIA_INLINE Vec3 sUnitSpherical(float inTheta, float inPhi);

    /// A set of vectors uniformly spanning the surface of a unit sphere, usable for debug purposes
    static const std::vector<Vec3> sUnitSphere;

    /// Get random unit vector
    template <class Random>
    static inline Vec3 sRandom(Random& inRandom);

    /// Get individual components
#if defined(TOPIA_USE_SSE)
    TOPIA_INLINE float GetX() const
    {
        return _mm_cvtss_f32(mValue);
    }
    TOPIA_INLINE float GetY() const { return mF32[1]; }
    TOPIA_INLINE float GetZ() const { return mF32[2]; }
#elif defined(TOPIA_USE_NEON)
    TOPIA_INLINE float GetX() const
    {
        return vgetq_lane_f32(mValue, 0);
    }
    TOPIA_INLINE float GetY() const { return vgetq_lane_f32(mValue, 1); }
    TOPIA_INLINE float GetZ() const { return vgetq_lane_f32(mValue, 2); }
#else
#error Undefined
#endif

    /// Set individual components
    TOPIA_INLINE void SetX(float inX) { mF32[0] = inX; }
    TOPIA_INLINE void SetY(float inY) { mF32[1] = inY; }
    TOPIA_INLINE void SetZ(float inZ) { mF32[2] = mF32[3] = inZ; } // Assure Z and W are the same

    /// Get float component by index
    TOPIA_INLINE float operator[](uint inCoordinate) const
    {
        ASSERT(inCoordinate < 3);
        return mF32[inCoordinate];
    }

    /// Set float component by index
    TOPIA_INLINE void SetComponent(uint inCoordinate, float inValue)
    {
        ASSERT(inCoordinate < 3);
        mF32[inCoordinate] = inValue;
        mValue = sFixW(mValue);
    } // Assure Z and W are the same

    /// Comparison
    TOPIA_INLINE bool operator==(Vec3Arg inV2) const;
    TOPIA_INLINE bool operator!=(Vec3Arg inV2) const { return !(*this == inV2); }

    /// Test if two vectors are close
    TOPIA_INLINE bool IsClose(Vec3Arg inV2, float inMaxDistSq = 1.0e-12f) const;

    /// Test if vector is near zero
    TOPIA_INLINE bool IsNearZero(float inMaxDistSq = 1.0e-12f) const;

    /// Test if vector is normalized
    TOPIA_INLINE bool IsNormalized(float inTolerance = 1.0e-6f) const;

    /// Test if vector contains NaN elements
    TOPIA_INLINE bool IsNaN() const;

    /// Multiply two float vectors (component wise)
    TOPIA_INLINE Vec3 operator*(Vec3Arg inV2) const;

    /// Multiply vector with float
    TOPIA_INLINE Vec3 operator*(float inV2) const;

    /// Multiply vector with float
    friend TOPIA_INLINE Vec3 operator*(float inV1, Vec3Arg inV2);

    /// Divide vector by float
    TOPIA_INLINE Vec3 operator/(float inV2) const;

    /// Multiply vector with float
    TOPIA_INLINE Vec3& operator*=(float inV2);

    /// Multiply vector with vector
    TOPIA_INLINE Vec3& operator*=(Vec3Arg inV2);

    /// Divide vector by float
    TOPIA_INLINE Vec3& operator/=(float inV2);

    /// Add two float vectors (component wise)
    TOPIA_INLINE Vec3 operator+(Vec3Arg inV2) const;

    /// Add two float vectors (component wise)
    TOPIA_INLINE Vec3& operator+=(Vec3Arg inV2);

    /// Negate
    TOPIA_INLINE Vec3 operator-() const;

    /// Subtract two float vectors (component wise)
    TOPIA_INLINE Vec3 operator-(Vec3Arg inV2) const;

    /// Add two float vectors (component wise)
    TOPIA_INLINE Vec3& operator-=(Vec3Arg inV2);

    /// Divide (component wise)
    TOPIA_INLINE Vec3 operator/(Vec3Arg inV2) const;

    /// Swizzle the elements in inV
    template <u32 SwizzleX, u32 SwizzleY, u32 SwizzleZ>
    TOPIA_INLINE Vec3 Swizzle() const;

    /// Replicate the X component to all components
    TOPIA_INLINE Vec4 SplatX() const;

    /// Replicate the Y component to all components
    TOPIA_INLINE Vec4 SplatY() const;

    /// Replicate the Z component to all components
    TOPIA_INLINE Vec4 SplatZ() const;

    /// Get index of component with lowest value
    TOPIA_INLINE int GetLowestComponentIndex() const;

    /// Get index of component with highest value
    TOPIA_INLINE int GetHighestComponentIndex() const;

    /// Return the absolute value of each of the components
    TOPIA_INLINE Vec3 Abs() const;

    /// Reciprocal vector (1 / value) for each of the components
    TOPIA_INLINE Vec3 Reciprocal() const;

    /// Cross product
    TOPIA_INLINE Vec3 Cross(Vec3Arg inV2) const;

    /// Dot product, returns the dot product in X, Y and Z components
    TOPIA_INLINE Vec3 DotV(Vec3Arg inV2) const;

    /// Dot product, returns the dot product in X, Y, Z and W components
    TOPIA_INLINE Vec4 DotV4(Vec3Arg inV2) const;

    /// Dot product
    TOPIA_INLINE float Dot(Vec3Arg inV2) const;

    /// Squared length of vector
    TOPIA_INLINE float LengthSq() const;

    /// Length of vector
    TOPIA_INLINE float Length() const;

    /// Normalize vector
    TOPIA_INLINE Vec3 Normalized() const;

    /// Normalize vector or return inZeroValue if the length of the vector is zero
    TOPIA_INLINE Vec3 NormalizedOr(Vec3Arg inZeroValue) const;

    /// Store 3 floats to memory
    TOPIA_INLINE void StoreFloat3(Float3* outV) const;

    /// Convert each component from a float to an int
    TOPIA_INLINE UVec4 ToInt() const;

    /// Reinterpret Vec3 as a UVec4 (doesn't change the bits)
    TOPIA_INLINE UVec4 ReinterpretAsInt() const;

    /// Get the minimum of X, Y and Z
    TOPIA_INLINE float ReduceMin() const;

    /// Get the maximum of X, Y and Z
    TOPIA_INLINE float ReduceMax() const;

    /// Component wise square root
    TOPIA_INLINE Vec3 Sqrt() const;

    /// Get normalized vector that is perpendicular to this vector
    TOPIA_INLINE Vec3 GetNormalizedPerpendicular() const;

    /// Get vector that contains the sign of each element (returns 1.0f if positive, -1.0f if negative)
    TOPIA_INLINE Vec3 GetSign() const;

    /// To String
    friend std::ostream& operator<<(std::ostream& inStream, Vec3Arg inV)
    {
        inStream << inV.mF32[0] << ", " << inV.mF32[1] << ", " << inV.mF32[2];
        return inStream;
    }

    /// Internal helper function that checks that W is equal to Z, so e.g. dividing by it should not generate div by 0
    TOPIA_INLINE void CheckW() const;

    /// Internal helper function that ensures that the Z component is replicated to the W component to prevent divisions by zero
    static TOPIA_INLINE Type sFixW(Type inValue);

    union
    {
        Type mValue;
        float mF32[4];
    };
};

static_assert(std::is_trivial<Vec3>(), "Is supposed to be a trivial type!");
}

#include "Vec3.inl"
