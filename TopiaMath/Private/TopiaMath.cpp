#include "TopiaMath.h"

namespace topia
{
	Mat44LookFrom::Mat44LookFrom(const Vec3& InPosition, const Vec3& InDirection, const Vec3& InUp)
	{
		const Vec3 ZAixs = InDirection.Normalized();
		const Vec3 YAixs = (InUp.Cross(ZAixs)).Normalized();
		// const Vec3 XAixs = ZAixs.Cross(YAixs);
		// const Vec3 YAixs = (ZAixs.Cross(InUp)).Normalized();
		const Vec3 XAixs = YAixs.Cross(ZAixs);

		SetColumn4(0, {XAixs, 0.0f});
		SetColumn4(1, {YAixs, 0.0f});
		SetColumn4(2, {ZAixs, 0.0f});
		SetColumn4(3, {-(InPosition.Dot(XAixs)), -(InPosition.Dot(YAixs)), -(InPosition.Dot(ZAixs)), 1.0f});
	}

	Mat44LookAt::Mat44LookAt(const Vec3& InPosition, const Vec3& InTargetPosition, const Vec3& InUp)
		: Mat44LookFrom(InPosition, InTargetPosition - InPosition, InUp)
	{
	}
} // namespace topia
