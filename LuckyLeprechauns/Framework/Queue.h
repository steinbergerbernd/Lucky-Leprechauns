#pragma once

#include "BasicCollection.h"

template <class T>
class Queue : BasicCollection<std::queue<T>>
{
public:
	void pop();
	void push(const T& item);
	void clear();
	bool empty() const;
	T& front();
	const T& front() const;
};


template <class T>
void Queue<T>::clear()
{
	lock();
	
	while (!collection.empty())
		collection.pop();

	unlock();
}


template <class T>
void Queue<T>::pop()
{
	lock();
	collection.pop();
	unlock();
}


template <class T>
void Queue<T>::push(const T& item)
{
	lock();
	collection.push(item);
	unlock();
}


template <class T>
bool Queue<T>::empty() const
{
	lock();
	bool empty = collection.empty();
	unlock();

	return empty;
}


template <class T>
T& Queue<T>::front()
{
	lock();
	T& item = collection.front();
	unlock();

	return item;
}


template <class T>
const T& Queue<T>::front() const
{
	lock();
	const T& item = collection.front();
	unlock();

	return item;
}