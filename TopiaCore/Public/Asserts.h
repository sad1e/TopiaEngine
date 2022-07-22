#pragma once

#include <stdarg.h>
#include <string>

#ifndef _CONSOLE
#include <debugapi.h>
#endif

#define ENABLE_DEBUG_CONSOLE 1

namespace topia
{
#ifdef ENABLE_DEBUG_CONSOLE
	inline void Print(const char* msg) { printf("%s", msg); }
	inline void Print(const wchar_t* msg) { wprintf(L"%ws", msg); }
#else
	inline void Print(const char* msg) { OutputDebugStringA(msg); }

	inline void Print(const wchar_t* msg) { OutputDebugString(msg); }
#endif

	inline void Printf(const char* format, ...)
	{
		char buffer[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
	}

	inline void Printf(const wchar_t* format, ...)
	{
		wchar_t buffer[256];
		va_list ap;
		va_start(ap, format);
		vswprintf(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
	}

#ifndef RELEASE
	inline void PrintSubMessage(const char* format, ...)
	{
		Print("--> ");
		char buffer[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
		Print("\n");
	}

	inline void PrintSubMessage(const wchar_t* format, ...)
	{
		Print("--> ");
		wchar_t buffer[256];
		va_list ap;
		va_start(ap, format);
		vswprintf(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
		Print("\n");
	}

	inline void PrintSubMessage(void) {}
#endif
} // namespace topia

#ifdef ERROR
#undef ERROR
#endif
#ifdef ASSERT
#undef ASSERT
#endif
#ifdef HALT
#undef HALT
#endif

#define HALT(...) ERROR(__VA_ARGS__) __debugbreak();

#ifdef RELEASE

#define CHECK(isTrue) (void)(isTrue)
#define ASSERT(isTrue, ...) (void)(isTrue)
#define WARN_ONCE_IF(isTrue, ...) (void)(isTrue)
#define WARN_ONCE_IF_NOT(isTrue, ...) (void)(isTrue)
#define ERROR(msg, ...)
#define DEBUGPRINT(msg, ...) \
	do                       \
	{                        \
	} while (0)
#define ASSERT_SUCCEEDED(hr, ...) (void)(hr)

#else // !RELEASE

#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)

#define CHECK(isFalse)                                                                                         \
	if (!(bool)(isFalse))                                                                                      \
	{                                                                                                          \
		topia::Print("\nCheck failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
		topia::PrintSubMessage("\'" #isFalse "\' is false");                                                   \
		__debugbreak();                                                                                        \
	}

#define ASSERT(isFalse, ...)                                                                                       \
	if (!(bool)(isFalse))                                                                                          \
	{                                                                                                              \
		topia::Print("\nAssertion failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
		topia::PrintSubMessage("\'" #isFalse "\' is false");                                                       \
		topia::PrintSubMessage(__VA_ARGS__);                                                                       \
		topia::Print("\n");                                                                                        \
		__debugbreak();                                                                                            \
	}

#define ASSERT_SUCCEEDED(hr, ...)                                                                                \
	if (FAILED(hr))                                                                                              \
	{                                                                                                            \
		topia::Print("\nHRESULT failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
		topia::PrintSubMessage("hr = 0x%08X", hr);                                                               \
		topia::PrintSubMessage(__VA_ARGS__);                                                                     \
		topia::Print("\n");                                                                                      \
		__debugbreak();                                                                                          \
	}

#define WARN_ONCE_IF(isTrue, ...)                                                                                    \
	{                                                                                                                \
		static bool s_TriggeredWarning = false;                                                                      \
		if ((bool)(isTrue) && !s_TriggeredWarning)                                                                   \
		{                                                                                                            \
			s_TriggeredWarning = true;                                                                               \
			topia::Print("\nWarning issued in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
			topia::PrintSubMessage("\'" #isTrue "\' is true");                                                       \
			topia::PrintSubMessage(__VA_ARGS__);                                                                     \
			topia::Print("\n");                                                                                      \
		}                                                                                                            \
	}

#define WARN_ONCE_IF_NOT(isTrue, ...) WARN_ONCE_IF(!(isTrue), __VA_ARGS__)

#define ERROR(...)                                                                                           \
	topia::Print("\nError reported in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
	topia::PrintSubMessage(__VA_ARGS__);                                                                     \
	topia::Print("\n");

#define DEBUGPRINT(msg, ...) topia::Printf(msg "\n", ##__VA_ARGS__);

#endif

#define BreakIfFailed(hr) \
	if (FAILED(hr))       \
	__debugbreak()
