#pragma once

#include <Platforms.h>
#include <Asserts.h>
#include <StringUtils.h>
#include <MiscMacros.h>
#include <Allocators.h>

#if defined(DEBUG) || defined(_DEBUG)
	#define TOPIA_DEBUG
#endif

#define __STRUCT__ struct
#define interface __STRUCT__

// Detect CPU architecture
#if defined(__x86_64__) || defined(_M_X64)
    #define TOPIA_CPU_X64
    #define TOPIA_USE_SSE

    #if (defined(__F16C__) || defined(__AVX2__)) && !defined(TOPIA_USE_F16C)
        #define TOPIA_USE_F16C
    #endif
    #if (defined(__LZCNT__) || defined(__AVX2__)) && !defined(TOPIA_USE_LZCNT)
        #define TOPIA_USE_LZCNT
    #endif
    #if defined(__AVX__) && !defined(TOPIA_USE_AVX)
        #define TOPIA_USE_AVX
    #endif
    #if defined(__AVX2__) && !defined(TOPIA_USE_AVX2)
        #define TOPIA_USE_AVX2
    #endif
    #if defined(__clang__)
        #if defined(__FMA__) && !defined(TOPIA_USE_FMADD)
            #define TOPIA_USE_FMADD
        #endif
    #elif defined(_MSC_VER)
        #if defined(__AVX2__) && !defined(TOPIA_USE_FMADD) // AVX2 also enables fused multiply add
            #define TOPIA_USE_FMADD
        #endif
    #else
        #error Undefined compiler
    #endif
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define TOPIA_CPU_ARM64
    #define TOPIA_USE_NEON
#else
    #error Unsupported CPU arch
#endif

#define TOPIA_STACK_ALLOC(n)		alloca(n)

#define TOPIA_INLINE __forceinline

// Standard types
using uint = uint32_t;
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

// Assert sizes of types
static_assert(sizeof(u8)    == 1, "Invalid size of uint8");
static_assert(sizeof(u16)   == 2, "Invalid size of uint16");
static_assert(sizeof(u32)   == 4, "Invalid size of uint32");
static_assert(sizeof(u64)   == 8, "Invalid size of uint64");
static_assert(sizeof(void*) == 8, "Invalid size of pointer");

#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include <sstream>
#include <functional>
#include <random>
#include <type_traits>

#include <EABase/eabase.h>
#include <EASTL/algorithm.h>
#include <EASTL/array.h>
#include <EASTL/algorithm.h>
#include <EASTL/atomic.h>
#include <EASTL/functional.h>
#include <EASTL/random.h>
#include <EASTL/string.h>
#include <EASTL/type_traits.h>
#include <EASTL/utility.h>
#include <EASTL/vector.h>

#if defined(TOPIA_USE_SSE)
    #include <immintrin.h>
#elif defined(TOPIA_USE_NEON)
    #include <arm_neon.h>
#endif

#include <HashCombine.h>

#include "Misc.inl"

#include <RefCounting.h>

// TODO: Use DirectX12.
#define ENABLE_RHI_D3D11  0
#define ENABLE_RHI_D3D12  1
#define ENABLE_RHI_VULKAN 0

