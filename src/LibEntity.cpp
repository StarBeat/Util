#include "LibEntity.h"
#include <assert.h>
namespace Util
{

LibEntity::LibEntity(const std::string& name)
{
	this->name = name;
	handle = NULL;
}

LibEntity::~LibEntity()
{
	unload();
}

void LibEntity::load()
{
	handle = (LIB_HANDLE)LIB_LOAD(name.c_str());
}

void LibEntity::unload()
{
	assert(handle);
	LIB_UNLOAD(handle);
}

void* LibEntity::getSymbol(const std::string& filed) const throw()
{
	assert(handle);
	return (void*)LIB_GETSYM(handle, filed.c_str());
}

}
