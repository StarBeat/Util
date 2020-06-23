#include "Rpc.h"
#include <iostream>
using namespace Util;
using namespace std;

void bindFunc(int id);
class ClassMem;
struct PersonInfo;

Rpc server(bindFunc);


void foo_1() 
{
	server.call("foo_1");
}

void foo_2(int arg1)
{
	cout << arg1 << endl;
	server.call("foo_4", 11, "rpc", 1.56565, true);
}

struct PersonInfo
{
	int age;
	std::string name;
	float height;

	friend Serializer& operator >> (Serializer& in, PersonInfo& d) {
		in >> d.age >> d.name >> d.height;
		return in;
	}
	friend Serializer& operator << (Serializer& out, PersonInfo d) {
		out << d.age << d.name << d.height;
		return out;
	}
};

void foo_4(int arg1, std::string s, double d, bool b)
{
	cout << arg1 << s << d << b <<endl;
	server.call("foo_3", 10, "rpc", 100, PersonInfo{ 10, "rpc", 170 });
}



class ClassMem
{
public:
	void bar(int arg1, std::string arg2, int arg3, PersonInfo s)
	{
		cout << arg1 << "--"<< arg2<<"__"<< arg3 <<
			"PersonInfo"<< s.age << "+" << s.name<<"+" << s.height<< endl;
	}
};

void bindFunc(int id)
{
	server.bind(id, "foo_1", foo_1);
	server.bind(id, "foo_2", foo_2);
	ClassMem s;
	server.bind(id, "foo_3", &ClassMem::bar, &s);
	server.bind(id, "foo_4", foo_4);
}
int main()
{
	server.asServer(1234);

	std::cout << "serverRun: " << std::endl;
	while (true)
	{
		server.update();
		 _sleep(10);
		 server.call("foo_2", 10);
	}
}