#pragma once
namespace Util
{
template <typename T>
 struct  Singleton
{
private:
	Singleton(const Singleton<T>&);
	Singleton operator=(const Singleton<T>&);
protected:
	static T* instance;
	Singleton(void)
	{
		instance = static_cast<T*>(this);
	}
public:
	
	~Singleton() = default;
	static T* GetInstance() { return instance; }
};
}
