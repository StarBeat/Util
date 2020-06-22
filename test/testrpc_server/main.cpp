#include "Rpc.h"
#include <iostream>
using namespace Util;
using namespace std;

Rpc server;


void foo_1() 
{
	server.call("foo_1");
}

void foo_2(int arg1)
{
	cout << arg1 << endl;
}

void foo_4(int arg1, std::string s, double d, bool b)
{
	cout << arg1 << s << d << b <<endl;
}

struct PersonInfo
{
	int age;
	std::string name;
	float height;

	friend Serializer& operator >> (Serializer & in, PersonInfo & d) {
		in >> d.age >> d.name >> d.height;
		return in;
	}
	friend Serializer& operator << (Serializer & out, PersonInfo d) {
		out << d.age << d.name << d.height;
		return out;
	}
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
	server.asServer(1234);
	server.bind("foo_1", foo_1);
	server.bind("foo_2", foo_2);
	ClassMem s;
	server.bind("foo_3", &ClassMem::bar, &s);
	server.bind("foo_4", foo_4);
	std::cout << "serverRun: " << std::endl;
	while (true)
	{
		server.update();
		 _sleep(10);
	}
}