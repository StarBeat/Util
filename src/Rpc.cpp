#include "Rpc.h"
#include <sstream>
#include <thread>
#include "xnet.h"

namespace Util
{
using std::ostringstream;
Rpc::Rpc()
{
}

Rpc::~Rpc()
{
	_net->shutdown();
	delete _net;
}

void Rpc::asClient(string& ip, int port) 
{
	_type = CSType::RPC_Client;
	_net = new SimpleNet(1);
	_net->connect(ip, port);
	_net->run();
}

void Rpc::asServer(int port)
{
	_type = CSType::RPC_Server;
	_net = new SimpleNet(64);
	_net->listen("", port);
	_net->run();
}

void Rpc::send(Message* msg)
{
	_net->send(msg->id, msg);
}

void Rpc::recv(Message** msg)
{
	_net->recv(msg);
}

void Rpc::serverRun()
{
	if (_type != CSType::RPC_Server)
	{
		return;
	}
	std::thread t([this] 
		{
			while (true)
			{
				Message* msg = nullptr;
				recv(&msg);
				ByteStream bs((char*)msg->data, msg->size);
				Serializer ds(bs);
				_net->recycleMessage(msg);
				std::string funcname;
				ds >> funcname;
				call_(funcname, ds.current(), ds.size());
			}
		});
	t.detach();
}

void Rpc::update()
{
	Message* msg = nullptr;
	recv(&msg);
	if (msg == nullptr)
		return;
	ByteStream bs((char*)msg->data, msg->size);
	Serializer ds(bs);
	_net->recycleMessage(msg);
	std::string funcname;
	ds >> funcname;
	call_(funcname, ds.current(), ds.size() - funcname.size());
}

void Rpc::call_(std::string name, const char* data, int len)
{
	if (_handlers.find(name) == _handlers.end())
	{
		return;
	}
	auto fun = _handlers[name];
	fun(data, len);
}

void Rpc::net_call(Serializer& ds)
{
	Message* msg  = _net->getMessage();
	msg->id = 0;
	msg->size = ds.size();
	memcpy(msg->data + sizeof(uint16_t), ds.data(), ds.size());
	send(msg);
	ds.clear();
}

}