#pragma once

#include <Topia.h>
#include "MathTypes.h"

namespace topia
{
	/**
	 * Holds a 4x4 matrix of floats, but supports also operations on the 3x3 upper left part of the matrix.
	 *
	 */
	class TOPIA_NODISCARD Mat44
	{
	public:
		// Underlying column type
		using Type = Vec4::Type;

		/// Constructor
		Mat44() = default; ///< Intentionally not initialized for performance reasons
		TOPIA_INLINE Mat44(Vec4Arg inC1, Vec4Arg inC2, Vec4Arg inC3, Vec4Arg inC4);
		Mat44(const Mat44& inM2) = default;
		TOPIA_INLINE Mat44(Type inC1, Type inC2, Type inC3, Type inC4);

		/// Zero matrix
		static TOPIA_INLINE Mat44 sZero();

		/// Identity matrix
		static TOPIA_INLINE Mat44 sIdentity();

		/// Matrix filled with NaN's
		static TOPIA_INLINE Mat44 sNaN();

		/// Load 16 floats from memory
		static TOPIA_INLINE Mat44 sLoadFloat4x4(const Float4* inV);

		/// Load 16 floats from memory, 16 bytes aligned
		static TOPIA_INLINE Mat44 sLoadFloat4x4Aligned(const Float4* inV);

		/// Rotate around X, Y or Z axis (angle in radians)
		static TOPIA_INLINE Mat44 sRotationX(float inX);
		static TOPIA_INLINE Mat44 sRotationY(float inY);
		static TOPIA_INLINE Mat44 sRotationZ(float inZ);

		/// Rotate around arbitrary axis
		static TOPIA_INLINE Mat44 sRotation(Vec3Arg inAxis, float inAngle);

		/// Rotate from quaternion
		static TOPIA_INLINE Mat44 sRotation(QuatArg inQuat);

		/// Get matrix that translates
		static TOPIA_INLINE Mat44 sTranslation(Vec3Arg inV);

		/// Get matrix that rotates and translates
		static TOPIA_INLINE Mat44 sRotationTranslation(QuatArg inR, Vec3Arg inT);

		/// Get inverse matrix of sRotationTranslation
		static TOPIA_INLINE Mat44 sInverseRotationTranslation(QuatArg inR, Vec3Arg inT);

		/// Get matrix that scales uniformly
		static TOPIA_INLINE Mat44 sScale(float inScale);

		/// Get matrix that scales (produces a matrix with (inV, 1) on its diagonal)
		static TOPIA_INLINE Mat44 sScale(Vec3Arg inV);

		/// Get outer product of inV and inV2 (equivalent to \f$inV1 \otimes inV2\f$)
		static TOPIA_INLINE Mat44 sOuterProduct(Vec3Arg inV1, Vec3Arg inV2);

		/// Get matrix that represents a cross product \f$A \times B = \text{sCrossProduct}(A) \: B\f$
		static TOPIA_INLINE Mat44 sCrossProduct(Vec3Arg inV);

		/// Returns matrix ML so that \f$ML(q) \: p = q \: p\f$ (where p and q are quaternions)
		static TOPIA_INLINE Mat44 sQuatLeftMultiply(QuatArg inQ);

		/// Returns matrix MR so that \f$MR(q) \: p = p \: q\f$ (where p and q are quaternions)
		static TOPIA_INLINE Mat44 sQuatRightMultiply(QuatArg inQ);

		/// Get float component by element index
		TOPIA_INLINE float operator()(uint inRow, uint inColumn) const
		{
			ASSERT(inRow < 4);
			ASSERT(inColumn < 4);
			return mCol[inColumn].mF32[inRow];
		}
		TOPIA_INLINE float& operator()(uint inRow, uint inColumn)
		{
			ASSERT(inRow < 4);
			ASSERT(inColumn < 4);
			return mCol[inColumn].mF32[inRow];
		}

		/// Comparsion
		TOPIA_INLINE bool operator==(Mat44Arg inM2) const;
		TOPIA_INLINE bool operator!=(Mat44Arg inM2) const { return !(*this == inM2); }

		/// Test if two matrices are close
		TOPIA_INLINE bool IsClose(Mat44Arg inM2, float inMaxDistSq = 1.0e-12f) const;

		/// Multiply matrix by matrix
		TOPIA_INLINE Mat44 operator*(Mat44Arg inM) const;

		/// Multiply vector by matrix
		TOPIA_INLINE Vec3 operator*(Vec3Arg inV) const;
		TOPIA_INLINE Vec4 operator*(Vec4Arg inV) const;

		/// Multiply vector by only 3x3 part of the matrix
		TOPIA_INLINE Vec3 Multiply3x3(Vec3Arg inV) const;

		/// Multiply vector by only 3x3 part of the transpose of the matrix (\f$result = this^T \: inV\f$)
		TOPIA_INLINE Vec3 Multiply3x3Transposed(Vec3Arg inV) const;

		/// Multiply 3x3 matrix by 3x3 matrix
		TOPIA_INLINE Mat44 Multiply3x3(Mat44Arg inM) const;

		/// Multiply transpose of 3x3 matrix by 3x3 matrix (\f$result = this^T \: inM\f$)
		TOPIA_INLINE Mat44 Multiply3x3LeftTransposed(Mat44Arg inM) const;

		/// Multiply 3x3 matrix by the transpose of a 3x3 matrix (\f$result = this \: inM^T\f$)
		TOPIA_INLINE Mat44 Multiply3x3RightTransposed(Mat44Arg inM) const;

		/// Multiply matrix with float
		TOPIA_INLINE Mat44 operator*(float inV) const;
		friend TOPIA_INLINE Mat44 operator*(float inV, Mat44Arg inM) { return inM * inV; }

		/// Multiply matrix with float
		TOPIA_INLINE Mat44& operator*=(float inV);

		/// Per element addition of matrix
		TOPIA_INLINE Mat44 operator+(Mat44Arg inM) const;

		/// Negate
		TOPIA_INLINE Mat44 operator-() const;

		/// Per element subtraction of matrix
		TOPIA_INLINE Mat44 operator-(Mat44Arg inM) const;

		/// Per element addition of matrix
		TOPIA_INLINE Mat44& operator+=(Mat44Arg inM);

		/// Access to the columns
		TOPIA_INLINE const Vec3 GetAxisX() const { return Vec3(mCol[0]); }
		TOPIA_INLINE void SetAxisX(Vec3Arg inV) { mCol[0] = Vec4(inV, 0.0f); }
		TOPIA_INLINE const Vec3 GetAxisY() const { return Vec3(mCol[1]); }
		TOPIA_INLINE void SetAxisY(Vec3Arg inV) { mCol[1] = Vec4(inV, 0.0f); }
		TOPIA_INLINE const Vec3 GetAxisZ() const { return Vec3(mCol[2]); }
		TOPIA_INLINE void SetAxisZ(Vec3Arg inV) { mCol[2] = Vec4(inV, 0.0f); }
		TOPIA_INLINE const Vec3 GetTranslation() const { return Vec3(mCol[3]); }
		TOPIA_INLINE void SetTranslation(Vec3Arg inV) { mCol[3] = Vec4(inV, 1.0f); }
		TOPIA_INLINE const Vec3 GetDiagonal3() const { return Vec3(mCol[0][0], mCol[1][1], mCol[2][2]); }
		TOPIA_INLINE void SetDiagonal3(Vec3Arg inV)
		{
			mCol[0][0] = inV.GetX();
			mCol[1][1] = inV.GetY();
			mCol[2][2] = inV.GetZ();
		}
		TOPIA_INLINE const Vec4 GetDiagonal4() const { return Vec4(mCol[0][0], mCol[1][1], mCol[2][2], mCol[3][3]); }
		TOPIA_INLINE void SetDiagonal4(Vec4Arg inV)
		{
			mCol[0][0] = inV.GetX();
			mCol[1][1] = inV.GetY();
			mCol[2][2] = inV.GetZ();
			mCol[3][3] = inV.GetW();
		}
		TOPIA_INLINE const Vec3 GetColumn3(uint inCol) const
		{
			ASSERT(inCol < 4);
			return Vec3(mCol[inCol]);
		}
		TOPIA_INLINE void SetColumn3(uint inCol, Vec3Arg inV)
		{
			ASSERT(inCol < 4);
			mCol[inCol] = Vec4(inV, inCol == 3 ? 1.0f : 0.0f);
		}
		TOPIA_INLINE const Vec4 GetColumn4(uint inCol) const
		{
			ASSERT(inCol < 4);
			return mCol[inCol];
		}
		TOPIA_INLINE void SetColumn4(uint inCol, Vec4Arg inV)
		{
			ASSERT(inCol < 4);
			mCol[inCol] = inV;
		}

		/// Store matrix to memory
		TOPIA_INLINE void StoreFloat4x4(Float4* outV) const;

		/// Transpose matrix
		TOPIA_INLINE Mat44 Transposed() const;

		/// Transpose 3x3 subpart of matrix
		TOPIA_INLINE Mat44 Transposed3x3() const;

		/// Inverse 4x4 matrix
		TOPIA_INLINE Mat44 Inversed() const;

		/// Inverse 4x4 matrix when it only contains rotation and translation
		TOPIA_INLINE Mat44 InversedRotationTranslation() const;

		/// Get the determinant of a 3x3 matrix
		TOPIA_INLINE float GetDeterminant3x3() const;

		/// Get the adjoint of a 3x3 matrix
		TOPIA_INLINE Mat44 Adjointed3x3() const;

		/// Inverse 3x3 matrix
		TOPIA_INLINE Mat44 Inversed3x3() const;

		/// Get rotation part only (note: retains the first 3 values from the bottom row)
		TOPIA_INLINE Mat44 GetRotation() const;

		/// Get rotation part only (note: also clears the bottom row)
		TOPIA_INLINE Mat44 GetRotationSafe() const;

		/// Updates the rotation part of this matrix (the first 3 columns)
		TOPIA_INLINE void SetRotation(Mat44Arg inRotation);

		/// Convert to quaternion
		TOPIA_INLINE const Quat GetQuaternion() const;

		/// Get matrix that transforms a direction with the same transform as this matrix (length is not preserved)
		TOPIA_INLINE const Mat44 GetDirectionPreservingMatrix() const
		{
			return GetRotation().Inversed3x3().Transposed3x3();
		}

		/// Scale a matrix: result = this * Mat44::sScale(inScale)
		TOPIA_INLINE Mat44 PreScaled(Vec3Arg inScale) const;

		/// Scale a matrix: result = Mat44::sScale(inScale) * this
		TOPIA_INLINE Mat44 PostScaled(Vec3Arg inScale) const;

		/// Decompose a matrix into a rotation & translation part and into a scale part so that:
		/// this = return_value * Mat44::sScale(outScale).
		/// This equation only holds when the matrix is orthogonal, if it is not the returned matrix
		/// will be made orthogonal using the modified Gram-Schmidt algorithm (see:
		/// https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process)
		TOPIA_INLINE Mat44 Decompose(Vec3& outScale) const;

		/// To String
		friend std::ostream& operator<<(std::ostream& inStream, Mat44Arg inM)
		{
			inStream << inM.mCol[0] << ", " << inM.mCol[1] << ", " << inM.mCol[2] << ", " << inM.mCol[3];
			return inStream;
		}

	private:
		Vec4 mCol[4]; ///< Column
	};

	static_assert(std::is_trivial<Mat44>(), "Is supposed to be a trivial type!");

} // namespace topia

#include "Mat44.inl"
