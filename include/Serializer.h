#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include "Defines.h"
namespace Util
{
class DLL_PUBLIC ByteStream final
{
public:
	ByteStream() { index = 0; }
	ByteStream(const char* s, size_t len) { index = 0; insert(s, len); }
	~ByteStream() = default;

	size_t capacity() { return buffer.capacity(); }
	const char* data() { return &buffer[0]; }
	size_t size() { return buffer.size(); }
	const char* current() { return &buffer[index]; }
	void offset(int o) { index += o; }
	void clear() { buffer.clear(); }
	void reset() { index = 0; }
	bool canRead(size_t len) { return index + len <= size(); }
	int find(char c)
	{
		auto itr = std::find(buffer.begin() + index, buffer.end(), c);
		if (itr != buffer.end())
		{
			return itr - (buffer.begin() + index);
		}
		return -1;
	}
	void insert(const char* c, size_t len)
	{
		for (size_t i = 0; i < len; i++)
		{
			buffer.push_back(c[i]);
		}

	}
private:
	std::vector<char> buffer;
	size_t index;
};

class DLL_PUBLIC Serializer final
{
public:
	Serializer() = default;
	Serializer(const ByteStream& s) { _io = s; }
	~Serializer() = default;

	size_t size() { return _io.size(); }
	const char* data() { return _io.data(); }
	const char* current() { return _io.current(); }
	void reset() { _io.reset(); }
	void clear() { _io.clear(); reset(); }
	template<typename Tuple, std::size_t Id>
	void getv(Serializer& ds, Tuple& t) {
		ds >> std::get<Id>(t);
	}

	template<typename Tuple, std::size_t... I>
	Tuple getTuple(std::index_sequence<I...>) {
		Tuple t;
		initializer_list<int>{((getv<Tuple, I>(*this, t)), 0)...};
		return t;
	}

	template<typename T>
	Serializer& operator>>(T& t)
	{
		size_t len = sizeof T;
		char* tmp = new char[len];
		if (_io.canRead(len))
		{
			memcpy(tmp, _io.current(), len);
			_io.offset(len);
			t = *reinterpret_cast<T*>(&tmp[0]);
		}
		delete[] tmp;
		return *this;
	}

	template<typename T>
	Serializer& operator<<(T t)
	{
		int len = sizeof T;
		char* tmp = new char[len];
		char* p = reinterpret_cast<char*>(&t);
		memcpy(tmp, p, len);
		_io.insert(tmp, len);
		delete[] tmp;
		return *this;
	}

	template<>
	Serializer& operator<<(std::string t)
	{
		size_t len = t.size();
		char* p = reinterpret_cast<char*>(&len);
		_io.insert(p, sizeof size_t);
		char* tmp = new char[len];
		memcpy(tmp, t.data(), len);
		_io.insert(tmp, len);
		delete tmp;
		return *this;

	}

	template<>
	Serializer& operator>>(std::string& t)
	{
		size_t si = sizeof(size_t);
		char* l = new char[si];
		memcpy(l, _io.current(), si);
		size_t len = *reinterpret_cast<uint16_t*>(&l[0]);
		_io.offset(si);
		delete l;
		t.insert(t.begin(), _io.current(), _io.current() + len);
		_io.offset(len);
		return *this;
	}
private:
	ByteStream _io;
};

}