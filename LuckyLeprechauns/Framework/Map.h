#pragma once

#include "BasicCollection.h"


template <class T, class U>
class Map : BasicCollection<std::map<T, U>>
{
public:
	const U& operator[](const T& key) const;
	U& operator[](const T& key);
};


template <class T, class U>
const U& Map<T, U>::operator[](const T& key) const
{
	lock();
	const U& value = collection[key];
	unlock();

	return value;
}


template <class T, class U>
U& Map<T, U>::operator[](const T& key)
{
	lock();
	U& value = collection[key];
	unlock();

	return value;
}