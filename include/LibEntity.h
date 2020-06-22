#pragma once
#include "Defines.h"
#include <string>
#if __WIN
#include <Windows.h>
#define LIB_HANDLE hInstance
#define LIB_LOAD( a ) LoadLibraryEx( a, NULL, 0 ) // we can not use LOAD_WITH_ALTERED_SEARCH_PATH with relative paths
#define LIB_GETSYM( a, b ) GetProcAddress( a, b )
#define LIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;
#elif __LIN
#include <dlfcn.h>
#define LIB_HANDLE void*
#define LIB_LOAD( a ) dlopen( a, RTLD_LAZY) // we can not use LOAD_WITH_ALTERED_SEARCH_PATH with relative paths
#define LIB_GETSYM( a, b ) dlsym( a, b )
#define LIB_UNLOAD( a ) dlclose( a )
#endif // __WIN
namespace Util
{
class LibEntity
{
protected:
	std::string name;//libxxx.dll ->xxx
	LIB_HANDLE handle;
public:
	LibEntity(const std::string& name);
	~LibEntity();
	void load();
	void unload();
	void* getSymbol(const std::string& filed) const throw();
	inline LIB_HANDLE GetHandle() { return handle; }
};
}
