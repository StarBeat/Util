#pragma once
#include <tuple>
#include <functional>
#include "coroutine_impl.hpp"
namespace Util
{
#define DLL_PUBLIC __declspec(dllexport)

struct coroutine_t
{
	co_id id;
	void** udr;
	void** udy;
};

class coroutine
{
public:
	template<typename ...T>
	static coroutine_t create(std::function<void(T...)>, T&...args);
	
	static coroutine_t create(std::function<void()>);
	static void destroy(coroutine_t ct);

	static void* resume(coroutine_t ct, void*);
	static void* yield(coroutine_t ct, void*);

	static void* resume(coroutine_t ct);
	static void* yield(coroutine_t ct);

	const static Status inline status(coroutine_t ct);

};

template<typename ...T>
coroutine_t coroutine::create(std::function<void(T...)> func, T&...args)
{
	void** tmp1 = new void* ();
	void** tmp2 = new void* ();
	*tmp1 = nullptr;
	*tmp2 = nullptr;
	return { _create(std::bind(func, std::forward<T...>(args...))), tmp1, tmp2};
}

coroutine_t coroutine::create(std::function<void()> func)
{
	void** tmp1 = new void* ();
	void** tmp2 = new void* ();
	*tmp1 = nullptr;
	*tmp2 = nullptr;
	return  { _create(func), tmp1, tmp2 };
}

void coroutine::destroy(coroutine_t ct)
{
	delete ct.udr;
	delete ct.udy;
	_destroy(ct.id);
}


void* coroutine::resume(coroutine_t ct, void* r)
{
	*ct.udr = r;
	void* rt = nullptr;
	_resume(ct.id);
	if (*ct.udy != nullptr)
	{
		rt = *ct.udy;
		*ct.udy = nullptr;
	}
	return rt;
}

void* coroutine::yield(coroutine_t ct, void* y)
{
	*ct.udy = y;
	void* rt = nullptr;
	_yield();
	if (*ct.udr != nullptr)
	{
		rt = *ct.udr;
		*ct.udr = nullptr;
	}
	return rt;
}

void* coroutine::resume(coroutine_t ct)
{
	void* rt = nullptr;
	_resume(ct.id);
	if (*ct.udy != nullptr)
	{
		rt = *ct.udy;
		*ct.udy = nullptr;
	}
	return rt;
}

void* coroutine::yield(coroutine_t ct)
{
	void* rt = nullptr;
	_yield();
	if (*ct.udr != nullptr)
	{
		rt = *ct.udr;
		*ct.udr = nullptr;
	}
	return rt;
}
const Status coroutine::status(coroutine_t ct)
{
	return _qureystatus(ct.id);
}

}