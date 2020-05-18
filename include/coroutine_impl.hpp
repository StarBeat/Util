#pragma once
#include <cassert>
#include <functional>
#include <vector>
#include <list>
#include "Defines.h"
#ifndef STACKSIZE
#define STACKSIZE (1024<<10)
#endif
#if defined(__WIN)
// windows
#include <Windows.h>
#elif defined(__LIN)
// linux
#include <ucontext.h>
#include <cstring>
#endif

using co_id = uint32_t;

enum Status
{
	COROUTINE_DEAD,
	COROUTINE_READY,
	COROUTINE_RUNNING,
	COROUTINE_SUSPEND,
};
#ifdef __WIN	
class CoroutineCtx
{
	Status status;
public:
	std::function<void()> func;
	LPVOID fiber;
	CoroutineCtx(std::function<void()> func) :fiber(nullptr), func(func)
	{
		status = Status::COROUTINE_READY;
	}
	inline void Run()
	{
		status = Status::COROUTINE_RUNNING;
		SwitchToFiber(fiber);
	}
	inline void Death()
	{
		status = Status::COROUTINE_DEAD;
	}
	inline void Suspend()
	{
		status = Status::COROUTINE_SUSPEND;
	}
	inline Status Status()
	{
		return status;
	}
	~CoroutineCtx()
	{
		if(nullptr != fiber)
		{
			DeleteFiber(fiber);
		}
	}
};

class Schedule
{
public:
	std::vector<CoroutineCtx*>cos;
	std::list<co_id> indexes;
	co_id current;
	LPVOID main_fiber;
	Schedule()
	{
		current = 0;
		main_fiber = ConvertThreadToFiber(nullptr);
	}
	~Schedule()
	{
		for (auto& i : cos)
			delete i;
	}

	void runmain()
	{
		current = 0;
		SwitchToFiber(main_fiber);
	}
};

thread_local static Schedule lc_schedule;

inline void __stdcall entry(LPVOID lpParameter)
{
	co_id id = lc_schedule.current;
	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	assert(ctx != nullptr);
	ctx->func();
	ctx->Death();
	lc_schedule.runmain();
}

inline co_id _create(std::function<void()> func)
{
	CoroutineCtx* ctx = new CoroutineCtx(func);
	if (lc_schedule.indexes.empty())
	{
		lc_schedule.cos.push_back(ctx);
		return lc_schedule.cos.size();
	}
	else
	{
		co_id id = lc_schedule.indexes.front();
		lc_schedule.indexes.pop_front();
		assert(lc_schedule.cos[id - 1] != nullptr);
		lc_schedule.cos[id - 1] = ctx;
		return id;
	}
}

inline void _destroy(co_id id)
{
	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	assert(ctx != nullptr);

	delete ctx;
	lc_schedule.cos[id - 1] = nullptr;
	lc_schedule.indexes.push_back(id);
}

inline int _resume(co_id id)
{
	assert(lc_schedule.current == 0);

	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	if (ctx == nullptr)
		return -1;

	switch (ctx->Status())
	{
	case Status::COROUTINE_READY:
		ctx->fiber = CreateFiber(STACKSIZE, entry, 0);
		lc_schedule.current = id;
		ctx->Run();
		break;
	case Status::COROUTINE_SUSPEND:
		lc_schedule.current = id;
		ctx->Run();
		break;
	case Status::COROUTINE_DEAD:
		return -2;
	case Status::COROUTINE_RUNNING:
		return -3;
	}
	return 0;
}

inline void _yield()
{
	co_id id = lc_schedule.current;
	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	assert(ctx != nullptr);
	ctx->Suspend();
	lc_schedule.runmain();
}

#elif	__LIN
class CoroutineCtx
{
	Status status;
public:
	std::function<void()> func;
	char* stack;
	size_t stack_size;
	size_t capacity;
	ucontext_t ctx;
	CoroutineCtx(std::function<void()> func) : func(func)
	{
		stack = nullptr;
		stack_size = 0;
		capacity = 0;
		status = Status::COROUTINE_READY;
	}
	inline void Run()
	{
		status = Status::COROUTINE_RUNNING;
	}
	inline void Death()
	{
		status = Status::COROUTINE_DEAD;
	}
	inline void Suspend()
	{
		status = Status::COROUTINE_SUSPEND;
	}
	Status Status()
	{
		return status;
	}

};

class Schedule
{
public:
	std::vector<CoroutineCtx*>cos;
	std::list<co_id> indexes;
	char stack[STACKSIZE];
	co_id current;
	ucontext_t main_ctx;
	Schedule()
	{
		current = 0;
	}
	~Schedule()
	{
		for (auto& i : cos)
			delete i;
	}
};

thread_local static Schedule lc_schedule;

inline void entry()
{
	co_id id = lc_schedule.current;
	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	ctx->func();
	ctx->Death();

	lc_schedule.current = 0;
	lc_schedule.indexes.push_back(id);//ucontext no removal or destruction api provided
}

inline co_id _create(std::function<void()> f)
{
	CoroutineCtx* ctx = new CoroutineCtx(f);

	if (lc_schedule.indexes.empty())
	{
		lc_schedule.cos.push_back(ctx);
		return lc_schedule.cos.size();
	}
	else
	{
		co_id id = lc_schedule.indexes.front();
		lc_schedule.indexes.pop_front();
		assert(lc_schedule.cos[id - 1] == nullptr);
		lc_schedule.cos[id - 1] = ctx;
		return id;
	}
}

inline void _destroy(co_id id)
{
	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	assert(ctx != nullptr);

	delete ctx;
	lc_schedule.cos[id - 1] = nullptr;
}

inline int _resume(co_id id)
{
	assert(lc_schedule.current == 0);

	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	if (ctx == nullptr)
		return -1;

	switch (ctx->Status())
	{
	case Status::COROUTINE_READY:
	{
		//initializes the structure to the currently active context.
		//When successful, getcontext() returns 0
		//On error, return -1 and set errno appropriately.
		getcontext(&ctx->ctx);

		//Before invoking makecontext(), the caller must allocate a new stack
		//for this context and assign its address to ucp->uc_stack,
		//and define a successor context and assign its address to ucp->uc_link.
		ctx->ctx.uc_stack.ss_sp = lc_schedule.stack;
		ctx->ctx.uc_stack.ss_size = STACKSIZE;
		ctx->ctx.uc_link = &lc_schedule.main_ctx;
		lc_schedule.current = id;
		ctx->Run();
		//When this context is later activated by swapcontext(), the function entry is called.
		//When this function returns, the  successor context is activated.
		//If the successor context pointer is NULL, the thread exits.
		makecontext(&ctx->ctx, reinterpret_cast<void (*)(void)>(entry), 0);

		//The swapcontext() function saves the current context,
		//and then activates the context of another.
		swapcontext(&lc_schedule.main_ctx, &ctx->ctx);
		break;
	}
	case Status::COROUTINE_SUSPEND:
		lc_schedule.current = id;
		memcpy(lc_schedule.stack + STACKSIZE - ctx->stack_size, ctx->stack, ctx->stack_size);
		ctx->Run();
		swapcontext(&lc_schedule.main_ctx, &ctx->ctx);
		break;
	case Status::COROUTINE_DEAD:
		return -2;
	case Status::COROUTINE_RUNNING:
		return -3;
	}
	return 0;
}

static void _save_stack(CoroutineCtx* ctx, char* top) { // 这个函数用于保存堆栈的信息
		// 需要注意的是，栈是从高位向低位生长
		// 所以，top表示的是其实是堆栈的底部，虽然它的地址最高
	char stack_bottom = 0; // 这个真的很有意思，stack_bottom的位置一定是栈栈的顶部，因为现在我们仍然在coroutine的堆栈里
	assert(top - &stack_bottom <= STACKSIZE); // top - &stack_bottom恰好是栈的大小
	if (ctx->capacity < top - &stack_bottom) { 
		// 前面的ctx->ctk->uc_stack.ss_sp设置成了schedule的stack
		free(ctx->stack);
		ctx->capacity = top - &stack_bottom;
		ctx->stack = (char*)malloc(ctx->capacity); // 申请保存栈的空间
	}
	ctx->stack_size = top - &stack_bottom;
	// 也就是说&stack_bottom在低位
	memcpy(ctx->stack, &stack_bottom, ctx->stack_size); 
}

inline void _yield()
{
	co_id id = lc_schedule.current;
	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	assert(ctx != nullptr);

	char* stack_top = lc_schedule.stack + STACKSIZE;
	_save_stack(ctx, stack_top);

	ctx->Suspend();

	lc_schedule.current = 0;
	swapcontext(&ctx->ctx, &lc_schedule.main_ctx);
}
#endif // __WIN

inline Status _qureystatus(co_id id)
{
	assert(id >= 0 && id < lc_schedule.cos.size());
	CoroutineCtx* ctx = lc_schedule.cos[id - 1];
	assert(ctx != nullptr);
	return ctx->Status();
}

