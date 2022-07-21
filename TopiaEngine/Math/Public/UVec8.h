#pragma once

#include <Topia.h>
#include "Vec8.h"

namespace topia
{
    class TOPIA_NODISCARD UVec8
    {
    public:
        UVec8() = default;
        UVec8(const UVec8& inRHS) = default;
        TOPIA_INLINE UVec8(__m256i inRHS) : mValue(inRHS) {}

        /// Set 256 bit vector from 2 128 bit vectors
        TOPIA_INLINE UVec8(UVec4Arg inLo, UVec4Arg inHi);

        /// Comparison
        TOPIA_INLINE bool operator==(UVec8Arg inV2) const;
        TOPIA_INLINE bool operator!=(UVec8Arg inV2) const { return !(*this == inV2); }

        /// Replicate int across all components
        static TOPIA_INLINE UVec8 sReplicate(u32 inV);

        /// Replicate the X component of inV to all components
        static TOPIA_INLINE UVec8 sSplatX(UVec4Arg inV);

        /// Replicate the Y component of inV to all components
        static TOPIA_INLINE UVec8 sSplatY(UVec4Arg inV);

        /// Replicate the Z component of inV to all components
        static TOPIA_INLINE UVec8 sSplatZ(UVec4Arg inV);

        /// Equals (component wise)
        static TOPIA_INLINE UVec8 sEquals(UVec8Arg inV1, UVec8Arg inV2);

        /// Component wise select, returns inV1 when highest bit of inControl = 0 and inV2 when highest bit of inControl = 1
        static TOPIA_INLINE UVec8 sSelect(UVec8Arg inV1, UVec8Arg inV2, UVec8Arg inControl);

        /// Logical or
        static TOPIA_INLINE UVec8 sOr(UVec8Arg inV1, UVec8Arg inV2);

        /// Logical xor
        static TOPIA_INLINE UVec8 sXor(UVec8Arg inV1, UVec8Arg inV2);

        /// Logical and
        static TOPIA_INLINE UVec8 sAnd(UVec8Arg inV1, UVec8Arg inV2);

        /// Get float component by index
        TOPIA_INLINE u32 operator[](uint inCoordinate) const
        {
            ASSERT(inCoordinate < 8);
            return mU32[inCoordinate];
        }
        TOPIA_INLINE u32& operator[](uint inCoordinate)
        {
            ASSERT(inCoordinate < 8);
            return mU32[inCoordinate];
        }

        /// 256 bit variant of Vec::Swizzle (no cross 128 bit lane swizzle)
        template <u32 SwizzleX, u32 SwizzleY, u32 SwizzleZ, u32 SwizzleW>
        TOPIA_INLINE UVec8 Swizzle() const;

        /// Test if any of the components are true (true is when highest bit of component is set)
        TOPIA_INLINE bool TestAnyTrue() const;

        /// Test if all components are true (true is when highest bit of component is set)
        TOPIA_INLINE bool TestAllTrue() const;

        /// Fetch the lower 128 bit from a 256 bit variable
        TOPIA_INLINE UVec4 LowerVec4() const;

        /// Fetch the higher 128 bit from a 256 bit variable
        TOPIA_INLINE UVec4 UpperVec4() const;

        /// Converts int to float
        TOPIA_INLINE Vec8 ToFloat() const;

        /// Shift all components by Count bits to the left (filling with zeros from the left)
        template <const uint Count>
        TOPIA_INLINE UVec8 LogicalShiftLeft() const;

        /// Shift all components by Count bits to the right (filling with zeros from the right)
        template <const uint Count>
        TOPIA_INLINE UVec8 LogicalShiftRight() const;

        /// Shift all components by Count bits to the right (shifting in the value of the highest bit)
        template <const uint Count>
        TOPIA_INLINE UVec8 ArithmeticShiftRight() const;

        union
        {
            __m256i mValue;
            u32 mU32[8];
        };
    };

    static_assert(std::is_trivial<UVec8>(), "Is supposed to be a trivial type!");

}

#include "UVec8.inl"
