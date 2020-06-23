#include <iostream>
#include "Rpc.h"
#include "coroutine.h"
#include "PluginsManager.h"
#include "EncryptionManager.h"
using namespace Util;

using namespace std;

void bindFunc(int id);
Rpc client(bindFunc);



void foo_1()
{
	cout << "s call" << endl;
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
	cout << arg1 << s << d << b << endl;
}
class ClassMem
{
public:
	void bar(int arg1, std::string arg2, int arg3, PersonInfo s)
	{
		cout << arg1 << "--" << arg2 << "__" << arg3 <<
			"PersonInfo" << s.age << "+" << s.name << "+" << s.height << endl;
	}
};

void bindFunc(int id)
{
	client.bind(id, "foo_1", foo_1);
	client.bind(id, "foo_2", foo_2);
	ClassMem s;
	client.bind(id, "foo_3", &ClassMem::bar, &s);
	client.bind(id, "foo_4", foo_4);
}

int main()
{
	string ip = "127.0.0.1";
	client.asClient(ip, 1234);

	std::cout << "clientRun: " << std::endl;
	PersonInfo  dd = { 10, "rpc", 170 };
	while (true)
	{
		client.update();
		//client.upDate();
		client.call("foo_3", 10, "rpc", 100, dd);
		client.call("foo_4",11, "rpc", 1.56565, true);
		client.call("foo_1");
		client.update();
		client.call("foo_2", 10);

		Sleep(1000);
	}
}