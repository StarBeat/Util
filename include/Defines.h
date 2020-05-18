#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
// windows
#define __WIN 1
#elif defined(__linux__) || defined(__linux)
// linux
#define __LIN 1
#endif

#if __WIN
#if _MSVC_LANG
#else
#define __CPP20 0
#endif //  __cplusplus
#define __CPP20 0
#endif // __WIN
