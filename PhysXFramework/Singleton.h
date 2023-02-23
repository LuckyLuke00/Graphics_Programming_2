#pragma once

// Meyers Singleton, threadsafe
template<typename T>
class Singleton
{
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton) = delete;

	static T* GetInstance()
	{
		static T* pStaticInstance{ new T{} };
		return pStaticInstance;
	}

protected:
	Singleton() = default;
};
