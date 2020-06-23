#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
// windows
#define __WIN 1
#elif defined(__linux__) || defined(__linux)
// linux
#define __LIN 1
#endif

#if __WIN
#ifdef _EXPORT
#define DLL_PUBLIC __declspec(dllexport)
#else
#define DLL_PUBLIC __declspec(dllimport)
#endif
#define DLL_PRIVATE
#elif __LIN
#define DLL_PUBLIC   __attribute__ ((visibility("default")))
#define DLL_PRIVATE __attribute__ ((visibility("hidden")))
#endif // __WIN