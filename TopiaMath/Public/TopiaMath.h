#pragma once

#include "MathUtils.h"
#include "Float2.h"
#include "Vec4.h"
#include "Mat44.h"

namespace topia
{
	constexpr float finfinity = std::numeric_limits<float>::infinity();
	constexpr double dinfinity = std::numeric_limits<double>::infinity();

	constexpr float pi = 3.14159265358979323846f;
	constexpr float invpi = 1.0f / pi;
	constexpr float twopi = pi * 2.0f;
	constexpr float halfpi = 1.57079632679489661923f;
	constexpr float quadpi = 0.78539816339744830961f;

	TOPIA_INLINE Vec3 Lerp(const Vec3& InStartValue, const Vec3& InEndValue, float InT)
	{
		return InStartValue * (1.0f - InT) + InEndValue * InT;
	}

	struct Mat44LookFrom : public Mat44
	{
		Mat44LookFrom(const Vec3& InPosition, const Vec3& InDirection, const Vec3& InUp);
	};

	struct Mat44LookAt : public Mat44LookFrom
	{
		Mat44LookAt(const Vec3& InPosition, const Vec3& InTargetPosition, const Vec3& InUp);
	};
} // namespace topia
