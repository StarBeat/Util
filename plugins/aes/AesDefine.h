#pragma once
#if __WIN
#ifdef AESEXPORT
#define AES_PUBLIC __declspec(dllexport)
#else
#define AES_PUBLIC __declspec(dllimport)
#endif
#elif __LIN
#define AES_PUBLIC   __attribute__ ((visibility("default")))
#endif