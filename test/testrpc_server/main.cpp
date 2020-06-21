#include "Rpc.h"
#include <iostream>
using namespace Util;
using namespace std;

Rpc server;


void foo_1() 
{
//	server.call("foo_1");
}

void foo_2(int arg1)
{
	cout << arg1 << endl;
}

struct PersonInfo
{
	int age;
	std::string name;
	float height;
};

class ClassMem
{
public:
	void bar(int arg1, std::string arg2, int arg3, PersonInfo s)
	{
		cout << arg1 << "--"<< arg2<<"__"<< arg3 <<
			"PersonInfo"<< s.age << "+" << s.name<<"+" << s.height<< endl;
	}
};
int main()
{
	server.asServer(5555);
	server.bind("foo_1", foo_1);
	server.bind("foo_2", foo_2);
	ClassMem s;
	server.bind("foo_3", &ClassMem::bar, &s);
	std::cout << "serverRun: " << std::endl;
	while (true)
	{
		server.upDate();
		 _sleep(10);
	}
}