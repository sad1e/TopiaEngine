#pragma once

#include <Topia.h>

#include "Float4.h"
#include "Swizzle.h"
#include "MathTypes.h"

namespace topia
{
	class TOPIA_NODISCARD Vec4
	{
	public:
		// Underlying vector type
#if defined(TOPIA_USE_SSE)
		using Type = __m128;
#elif defined(TOPIA_USE_NEON)
		using Type = float32x4_t;
#else
#error Undefined
#endif

		/// Constructor
		Vec4() = default; ///< Intentionally not initialized for performance reasons
		Vec4(const Vec4 &inRHS) = default;
		explicit TOPIA_INLINE Vec4(Vec3Arg inRHS); ///< WARNING: W component undefined!
		TOPIA_INLINE Vec4(Vec3Arg inRHS, float inW);
		TOPIA_INLINE Vec4(Type inRHS) : mValue(inRHS) {}

		/// Create a vector from 4 components
		TOPIA_INLINE Vec4(float inX, float inY, float inZ, float inW);

		/// Vector with all zeros
		static TOPIA_INLINE Vec4 sZero();

		/// Vector with all NaN's
		static TOPIA_INLINE Vec4 sNaN();

		/// Replicate inV across all components
		static TOPIA_INLINE Vec4 sReplicate(float inV);

		/// Load 4 floats from memory
		static TOPIA_INLINE Vec4 sLoadFloat4(const Float4 *inV);

		/// Load 4 floats from memory, 16 bytes aligned
		static TOPIA_INLINE Vec4 sLoadFloat4Aligned(const Float4 *inV);

		/// Gather 4 floats from memory at inBase + inOffsets[i] * Scale
		template <const int Scale>
		static TOPIA_INLINE Vec4 sGatherFloat4(const float *inBase, UVec4Arg inOffsets);

		/// Return the minimum value of each of the components
		static TOPIA_INLINE Vec4 sMin(Vec4Arg inV1, Vec4Arg inV2);

		/// Return the maximum of each of the components
		static TOPIA_INLINE Vec4 sMax(Vec4Arg inV1, Vec4Arg inV2);

		/// Equals (component wise)
		static TOPIA_INLINE UVec4 sEquals(Vec4Arg inV1, Vec4Arg inV2);

		/// Less than (component wise)
		static TOPIA_INLINE UVec4 sLess(Vec4Arg inV1, Vec4Arg inV2);

		/// Less than or equal (component wise)
		static TOPIA_INLINE UVec4 sLessOrEqual(Vec4Arg inV1, Vec4Arg inV2);

		/// Greater than (component wise)
		static TOPIA_INLINE UVec4 sGreater(Vec4Arg inV1, Vec4Arg inV2);

		/// Greater than or equal (component wise)
		static TOPIA_INLINE UVec4 sGreaterOrEqual(Vec4Arg inV1, Vec4Arg inV2);

		/// Calculates inMul1 * inMul2 + inAdd
		static TOPIA_INLINE Vec4 sFusedMultiplyAdd(Vec4Arg inMul1, Vec4Arg inMul2, Vec4Arg inAdd);

		/// Component wise select, returns inV1 when highest bit of inControl = 0 and inV2 when highest bit of inControl
		/// = 1
		static TOPIA_INLINE Vec4 sSelect(Vec4Arg inV1, Vec4Arg inV2, UVec4Arg inControl);

		/// Logical or (component wise)
		static TOPIA_INLINE Vec4 sOr(Vec4Arg inV1, Vec4Arg inV2);

		/// Logical xor (component wise)
		static TOPIA_INLINE Vec4 sXor(Vec4Arg inV1, Vec4Arg inV2);

		/// Logical and (component wise)
		static TOPIA_INLINE Vec4 sAnd(Vec4Arg inV1, Vec4Arg inV2);

		/// Sort the four elements of ioValue and sort ioIndex at the same time.
		/// Based on a sorting network: http://en.wikipedia.org/wiki/Sorting_network
		static TOPIA_INLINE void sSort4(Vec4 &ioValue, UVec4 &ioIndex);

		/// Reverse sort the four elements of ioValue (highest first) and sort ioIndex at the same time.
		/// Based on a sorting network: http://en.wikipedia.org/wiki/Sorting_network
		static TOPIA_INLINE void sSort4Reverse(Vec4 &ioValue, UVec4 &ioIndex);

		/// Get individual components
#if defined(TOPIA_USE_SSE)
		TOPIA_INLINE float GetX() const { return _mm_cvtss_f32(mValue); }
		TOPIA_INLINE float GetY() const { return mF32[1]; }
		TOPIA_INLINE float GetZ() const { return mF32[2]; }
		TOPIA_INLINE float GetW() const { return mF32[3]; }
#elif defined(TOPIA_USE_NEON)
		TOPIA_INLINE float GetX() const { return vgetq_lane_f32(mValue, 0); }
		TOPIA_INLINE float GetY() const { return vgetq_lane_f32(mValue, 1); }
		TOPIA_INLINE float GetZ() const { return vgetq_lane_f32(mValue, 2); }
		TOPIA_INLINE float GetW() const { return vgetq_lane_f32(mValue, 3); }
#else
#error Undefined
#endif

		/// Set individual components
		TOPIA_INLINE void SetX(float inX) { mF32[0] = inX; }
		TOPIA_INLINE void SetY(float inY) { mF32[1] = inY; }
		TOPIA_INLINE void SetZ(float inZ) { mF32[2] = inZ; }
		TOPIA_INLINE void SetW(float inW) { mF32[3] = inW; }

		/// Get float component by index
		TOPIA_INLINE float operator[](uint inCoordinate) const
		{
			ASSERT(inCoordinate < 4);
			return mF32[inCoordinate];
		}
		TOPIA_INLINE float &operator[](uint inCoordinate)
		{
			ASSERT(inCoordinate < 4);
			return mF32[inCoordinate];
		}

		/// Comparison
		TOPIA_INLINE bool operator==(Vec4Arg inV2) const;
		TOPIA_INLINE bool operator!=(Vec4Arg inV2) const { return !(*this == inV2); }

		/// Test if two vectors are close
		TOPIA_INLINE bool IsClose(Vec4Arg inV2, float inMaxDistSq = 1.0e-12f) const;

		/// Test if vector is normalized
		TOPIA_INLINE bool IsNormalized(float inTolerance = 1.0e-6f) const;

		/// Test if vector contains NaN elements
		TOPIA_INLINE bool IsNaN() const;

		/// Multiply two float vectors (component wise)
		TOPIA_INLINE Vec4 operator*(Vec4Arg inV2) const;

		/// Multiply vector with float
		TOPIA_INLINE Vec4 operator*(float inV2) const;

		/// Multiply vector with float
		friend TOPIA_INLINE Vec4 operator*(float inV1, Vec4Arg inV2);

		/// Divide vector by float
		TOPIA_INLINE Vec4 operator/(float inV2) const;

		/// Multiply vector with float
		TOPIA_INLINE Vec4 &operator*=(float inV2);

		/// Multiply vector with vector
		TOPIA_INLINE Vec4 &operator*=(Vec4Arg inV2);

		/// Divide vector by float
		TOPIA_INLINE Vec4 &operator/=(float inV2);

		/// Add two float vectors (component wise)
		TOPIA_INLINE Vec4 operator+(Vec4Arg inV2) const;

		/// Add two float vectors (component wise)
		TOPIA_INLINE Vec4 &operator+=(Vec4Arg inV2);

		/// Negate
		TOPIA_INLINE Vec4 operator-() const;

		/// Subtract two float vectors (component wise)
		TOPIA_INLINE Vec4 operator-(Vec4Arg inV2) const;

		/// Add two float vectors (component wise)
		TOPIA_INLINE Vec4 &operator-=(Vec4Arg inV2);

		/// Divide (component wise)
		TOPIA_INLINE Vec4 operator/(Vec4Arg inV2) const;

		/// Swizzle the elements in inV
		template <u32 SwizzleX, u32 SwizzleY, u32 SwizzleZ, u32 SwizzleW>
		TOPIA_INLINE Vec4 Swizzle() const;

		/// Replicate the X component to all components
		TOPIA_INLINE Vec4 SplatX() const;

		/// Replicate the Y component to all components
		TOPIA_INLINE Vec4 SplatY() const;

		/// Replicate the Z component to all components
		TOPIA_INLINE Vec4 SplatZ() const;

		/// Replicate the W component to all components
		TOPIA_INLINE Vec4 SplatW() const;

		/// Return the absolute value of each of the components
		TOPIA_INLINE Vec4 Abs() const;

		/// Reciprocal vector (1 / value) for each of the components
		TOPIA_INLINE Vec4 Reciprocal() const;

		/// Dot product, returns the dot product in X, Y and Z components
		TOPIA_INLINE Vec4 DotV(Vec4Arg inV2) const;

		/// Dot product
		TOPIA_INLINE float Dot(Vec4Arg inV2) const;

		/// Squared length of vector
		TOPIA_INLINE float LengthSq() const;

		/// Length of vector
		TOPIA_INLINE float Length() const;

		/// Normalize vector
		TOPIA_INLINE Vec4 Normalized() const;

		/// Store 4 floats to memory
		TOPIA_INLINE void StoreFloat4(Float4 *outV) const;

		/// Convert each component from a float to an int
		TOPIA_INLINE UVec4 ToInt() const;

		/// Reinterpret Vec4 as a UVec4 (doesn't change the bits)
		TOPIA_INLINE UVec4 ReinterpretAsInt() const;

		/// Store if X is negative in bit 0, Y in bit 1, Z in bit 2 and W in bit 3
		TOPIA_INLINE int GetSignBits() const;

		/// Get the minimum of X, Y, Z and W
		TOPIA_INLINE float ReduceMin() const;

		/// Get the maximum of X, Y, Z and W
		TOPIA_INLINE float ReduceMax() const;

		/// Component wise square root
		TOPIA_INLINE Vec4 Sqrt() const;

		/// Get vector that contains the sign of each element (returns 1.0f if positive, -1.0f if negative)
		TOPIA_INLINE Vec4 GetSign() const;

		/// To String
		friend std::ostream &operator<<(std::ostream &inStream, Vec4Arg inV)
		{
			inStream << inV.mF32[0] << ", " << inV.mF32[1] << ", " << inV.mF32[2] << ", " << inV.mF32[3];
			return inStream;
		}

		union
		{
			Type mValue;
			float mF32[4];
		};
	};

	static_assert(std::is_trivial<Vec4>(), "Is supposed to be a trivial type!");

} // namespace topia

#include "Vec4.inl"