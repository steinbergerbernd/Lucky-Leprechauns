#pragma once

#include "BasicCollection.h"

template <class T>
class List : BasicCollection<std::list<T>>
{
public:
	T& back();
	const T& back() const;

	void push_back(const T& item);
	void pop_back();
};


template <class T>
T& List<T>::back()
{
	lock();
	T& value = collection.back();
	unlock();

	return value;
}


template <class T>
const T& List<T>::back() const
{
	lock();
	T& value = collection.back();
	unlock();

	return value;
}


template <class T>
void List<T>::push_back(const T& item)
{
	lock();
	collection.push_back(item);
	unlock();
}


template <class T>
void List<T>::pop_back()
{
	lock();
	collection.pop_back();
	unlock();
}