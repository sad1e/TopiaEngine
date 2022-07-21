#pragma once

namespace topia
{
	template <typename T>
	T Align(T InSize, T InAlignment)
	{
		return (InSize + InAlignment - 1) & ~(InAlignment - 1);
	}

	template <typename T, typename U>
	TOPIA_NODISCARD bool ArraysAreDifferent(const T& a, const U& b)
	{
		if (a.size() != b.size())
			return true;

		for (u32 i = 0; i < u32(a.size()); i++)
		{
			if (a[i] != b[i])
				return true;
		}

		return false;
	}

	template <typename T, typename U>
	TOPIA_NODISCARD u32 ArrayDifferenceMask(const T& a, const U& b)
	{
		ASSERT(a.size() <= 32);
		ASSERT(b.size() <= 32);

		if (a.size() != b.size())
			return ~0u;

		u32 mask = 0;
		for (u32 i = 0; i < u32(a.size()); i++)
		{
			if (a[i] != b[i])
				mask |= (1 << i);
		}

		return mask;
	}

	inline u32 HashToU32(size_t InHash) { return u32(InHash) ^ (u32(InHash >> 32)); }

	// A type cast that is safer than static_cast in debug builds, and is a simple static_cast in release builds.
	// Used for downcasting various ISomething* pointers to their implementation classes in the backends.
	template <typename T, typename U>
	T CheckedCast(U u)
	{
		static_assert(!std::is_same<T, U>::value, "Redundant checked_cast");
#ifdef _DEBUG
		if (!u)
			return nullptr;
		T t = dynamic_cast<T>(u);
		if (!t)
			ASSERT(!"Invalid type cast"); // NOLINT(clang-diagnostic-string-conversion)
		return t;
#else
		return static_cast<T>(u);
#endif
	}
} // namespace topia