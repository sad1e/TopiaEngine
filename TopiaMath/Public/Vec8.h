#pragma once

#include <Topia.h>
#include "MathTypes.h"

namespace topia
{

    class TOPIA_NODISCARD Vec8
    {
    public:
        Vec8() = default;
        Vec8(const Vec8& inRHS) = default;
        TOPIA_INLINE Vec8(__m256 inRHS) : mValue(inRHS) {}

        /// Set 256 bit vector from 2 128 bit vectors
        TOPIA_INLINE Vec8(Vec4Arg inLo, Vec4Arg inHi);

        /// Vector with all zeros
        static TOPIA_INLINE Vec8 sZero();

        /// Replicate across all components
        static TOPIA_INLINE Vec8 sReplicate(float inV);

        /// Replicate the X component of inV to all components
        static TOPIA_INLINE Vec8 sSplatX(Vec4Arg inV);

        /// Replicate the Y component of inV to all components
        static TOPIA_INLINE Vec8 sSplatY(Vec4Arg inV);

        /// Replicate the Z component of inV to all components
        static TOPIA_INLINE Vec8 sSplatZ(Vec4Arg inV);

        /// Calculates inMul1 * inMul2 + inAdd
        static TOPIA_INLINE Vec8 sFusedMultiplyAdd(Vec8Arg inMul1, Vec8Arg inMul2, Vec8Arg inAdd);

        /// Component wise select, returns inV1 when highest bit of inControl = 0 and inV2 when highest bit of inControl = 1
        static TOPIA_INLINE Vec8 sSelect(Vec8Arg inV1, Vec8Arg inV2, UVec8Arg inControl);

        /// Component wise min
        static TOPIA_INLINE Vec8 sMin(Vec8Arg inV1, Vec8Arg inV2);

        /// Component wise max
        static TOPIA_INLINE Vec8 sMax(Vec8Arg inV1, Vec8Arg inV2);

        /// Less than
        static TOPIA_INLINE UVec8 sLess(Vec8Arg inV1, Vec8Arg inV2);

        /// Greater than
        static TOPIA_INLINE UVec8 sGreater(Vec8Arg inV1, Vec8Arg inV2);

        /// Load from memory
        static TOPIA_INLINE Vec8 sLoadFloat8(const float* inV);

        /// Load 8 floats from memory, 32 bytes aligned
        static TOPIA_INLINE Vec8 sLoadFloat8Aligned(const float* inV);

        /// Get float component by index
        TOPIA_INLINE float operator[](uint inCoordinate) const
        {
            ASSERT(inCoordinate < 8);
            return mF32[inCoordinate];
        }
        TOPIA_INLINE float& operator[](uint inCoordinate)
        {
            ASSERT(inCoordinate < 8);
            return mF32[inCoordinate];
        }

        /// Multiply two float vectors
        TOPIA_INLINE Vec8 operator*(Vec8Arg inV2) const;

        /// Multiply vector by float
        TOPIA_INLINE Vec8 operator*(float inV2) const;

        /// Add two float vectors
        TOPIA_INLINE Vec8 operator+(Vec8Arg inV2) const;

        /// Subtract two float vectors
        TOPIA_INLINE Vec8 operator-(Vec8Arg inV2) const;

        /// Divide
        TOPIA_INLINE Vec8 operator/(Vec8Arg inV2) const;

        /// Reciprocal vector
        TOPIA_INLINE Vec8 Reciprocal() const;

        /// 256 bit variant of Vec::Swizzle (no cross 128 bit lane swizzle)
        template <u32 SwizzleX, u32 SwizzleY, u32 SwizzleZ, u32 SwizzleW>
        TOPIA_INLINE Vec8 Swizzle() const;

        /// Get absolute value of all components
        TOPIA_INLINE Vec8 Abs() const;

        /// Fetch the lower 128 bit from a 256 bit variable
        TOPIA_INLINE Vec4 LowerVec4() const;

        /// Fetch the higher 128 bit from a 256 bit variable
        TOPIA_INLINE Vec4 UpperVec4() const;

        /// Get the minimum value of the 8 floats
        TOPIA_INLINE float ReduceMin() const;

        union
        {
            __m256 mValue;
            float mF32[8];
        };
    };

    static_assert(std::is_trivial<Vec8>(), "Is supposed to be a trivial type!");

}

#include "Vec8.inl"