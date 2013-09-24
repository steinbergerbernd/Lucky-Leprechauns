#pragma once

#define WIN32_LEAN_AND_MEAN

#include "STL.h"
#include "Windows.h"

template <class T>
class BasicCollection
{
public:
	BasicCollection();
	virtual ~BasicCollection() = 0;

protected:
	void lock() const;
	void unlock() const;

	T collection;

private:
	HANDLE mutex;
};


template <class T>
BasicCollection<T>::BasicCollection()
{
	mutex = ::CreateMutexA(0, false, 0);
}


template <class T>
BasicCollection<T>::~BasicCollection()
{
	::CloseHandle(mutex);
}


template <class T>
void BasicCollection<T>::lock() const
{
	::WaitForSingleObject(mutex, INFINITE);
}


template <class T>
void BasicCollection<T>::unlock() const
{
	::ReleaseMutex(mutex);
}
