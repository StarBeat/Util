#include "Rpc.h"
#include <sstream>
#include <thread>
namespace Util
{
using std::ostringstream;
Rpc::Rpc()
{
}

Rpc::~Rpc()
{
}

void Rpc::asClient(string& ip, int port) 
{
	_type = CSType::RPC_Client;
	_socket = std::unique_ptr<zmq::socket_t, std::function<void(zmq::socket_t*)>>(new zmq::socket_t(m_context, ZMQ_REQ), [](zmq::socket_t* sock) { sock->close(); delete sock; sock = nullptr; });
	ostringstream os;
	os << "tcp://" << ip << ":" << port;
	_socket->connect(os.str());
}

void Rpc::asServer(int port)
{
	_type = CSType::RPC_Server;
	_socket = std::unique_ptr<zmq::socket_t, std::function<void(zmq::socket_t*)>>(new zmq::socket_t(m_context, ZMQ_REP), [](zmq::socket_t* sock) { sock->close(); delete sock; sock = nullptr; });
	ostringstream os;
	os << "tcp://*:" << port;
	_socket->bind(os.str());
}

void Rpc::sendNoblock(zmq::message_t& msg)
{
	_socket->send(msg, ZMQ_DONTWAIT);
}

void Rpc::send(zmq::message_t& msg)
{
	_socket->send(msg);
}

void Rpc::recvNoblock(zmq::message_t& msg)
{
	_socket->recv(&msg, ZMQ_DONTWAIT);
}

void Rpc::recv(zmq::message_t& msg)
{
	_socket->recv(&msg);
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
				zmq::message_t msg;
				recv(msg);
				ByteStream bs((char*)msg.data(), msg.size());
				Serializer ds(bs);

				std::string funcname;
				ds >> funcname;
				call_(funcname, ds.current(), ds.size());
			}
		});
	t.detach();
}

void Rpc::upDate()
{
	zmq::message_t msg;
	recv(msg);
	if (msg.size() == 0)
		return;
	ByteStream bs((char*)msg.data(), msg.size());
	Serializer ds(bs);

	std::string funcname;
	ds >> funcname;
	call_(funcname, ds.current(), ds.size());
}

void Rpc::call_(std::string name, const char* data, int len)
{
	if (_handlers.find(name) == _handlers.end())
	{
	}
	auto fun = _handlers[name];
	fun(data, len);
}

void Rpc::net_call(Serializer& ds)
{
	zmq::message_t request(ds.size() + 1);
	memcpy(request.data(), ds.data(), ds.size());
	send(request);
	ds.clear();
}

}