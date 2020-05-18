#include "coroutine.h"
#include <iostream>

using namespace Util;

int main()
{
	coroutine_t co1, co2;

	co1 = coroutine ::create([&](void)->void {
		std::cout << "co1+" << std::endl;
		auto r1 = (int*)coroutine::yield(co1, (void*)1);
		if (r1 != nullptr)
			std::cout << "co1+" << *r1<< std::endl;

		});
	std::function<void(int)> fu = [&](int i) {
		std::cout << "co2+" << i <<std::endl;
		coroutine::yield(co2);
	}; 
	int* i = new int(10);
	co2 = coroutine::create(fu, *i);
	while (coroutine::status(co1) && coroutine::status(co1))
	{
		auto y1 =(int*)coroutine::resume(co1, (void*)i);
		if(y1!=nullptr)
			std::cout <<"main+" << reinterpret_cast<int>(y1)<< std::endl;
		coroutine::resume(co2);

	}

	return 0;
}