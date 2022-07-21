#pragma once

#ifdef TOPIAENGINE_EXPORTS
	#define TOPIAENGINE_API _declspec(dllexport)
#else
	#define TOPIAENGINE_API _declspec(dllimport)
#endif

#ifndef TOPIAENGINE_INLINE
	#define TOPIA_INLINE __forceinline
#endif

#define TOPIA_CONSTEXPR constexpr
#define TOPIA_NODISCARD [[nodiscard]]


#define ENUM_CLASS_FLAG_OPERATORS(T)									\
	TOPIA_INLINE T operator | (T a, T b) { return T(u32(a) | u32(b)); } \
	TOPIA_INLINE T operator & (T a, T b) { return T(u32(a) & u32(b)); } \
	TOPIA_INLINE T operator ^ (T a, T b) { return T(u32(a) ^ u32(b)); } \
	TOPIA_INLINE T operator ~(T a) { return T(~u32(a)); }				\
	TOPIA_INLINE bool operator!(T a) { return u32(a) == 0; }			\
	TOPIA_INLINE bool operator == (T a, u32 b) { return u32(a) == b; }  \
	TOPIA_INLINE bool operator != (T a, u32 b) { return u32(a) != b; }

#define TO_STRING( x ) #x