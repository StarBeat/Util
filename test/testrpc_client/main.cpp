#include <iostream>
#include "Rpc.h"
#include "coroutine.h"
#include "PluginsManager.h"
#include "EncryptionManager.h"
using namespace Util;

using namespace std;


Rpc client;


void foo_1()
{
	zmq::message_t request;
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
		cout << arg1 << "--" << arg2 << "__" << arg3 <<
			"PersonInfo" << s.age << "+" << s.name << "+" << s.height << endl;
	}
};
int main()
{
	string ip = "127.0.0.1";
	client.asClient(ip, 5555);
	client.setTimeout(2000);
	//client.bind("foo_1", foo_1);
	//client.bind("foo_2", foo_2);
	//ClassMem s;
	//client.bind("foo_3", &ClassMem::bar, &s);
	std::cout << "clientRun: " << std::endl;
	PersonInfo  dd = { 10, "rpc", 170 };
	while (true)
	{
	//	foo_1();
		//client.upDate();
		client.call("foo_1");
		zmq::message_t m;
		client.recv(m);
		client.call("foo_2", 10);
		client.recv(m);

		client.call("foo_3", 10, "rpc", 100, dd);
		client.recv(m);

		Sleep(1000);
	}
}