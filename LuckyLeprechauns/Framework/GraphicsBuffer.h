#pragma once

#include <vector>

#include "Exception.h"
#include "Resource.h"

template <class T, class DESC>
class GraphicsBuffer : public Resource<T>
{
public:
	virtual ~GraphicsBuffer() = 0;

	DESC getDesc() const;

	template <class TF>
	void getData(TF* data) const;

	template <class TF>
	void setData(const std::vector<TF>& data, unsigned long flags = 0) const;

	template <class TF, int N>
	void setData(TF (&data) [N], unsigned long flags = 0) const;

	template <class TF>
	void setData(TF* data, int size, unsigned long flags = 0) const;

protected:
	virtual const char* getLockFailedException() const = 0;
	virtual const char* getUnlockFailedException() const = 0;
	virtual const char* getGetDescFailedException() const = 0;


private:
	void lock(VOID **ppbData, unsigned long flags = 0, unsigned offsetToLock = 0, unsigned sizeToLock = 0) const;
	void unlock() const;
};


template <class T, class DESC>
GraphicsBuffer<T, DESC>::~GraphicsBuffer()
{
}


template <class T, class DESC>
DESC GraphicsBuffer<T, DESC>::getDesc() const
{
	DESC desc;

	if (!resource || resource->GetDesc(&desc) != D3D_OK)
		throw Exception(getGetDescFailedException());

	return desc;
}


template <class T, class DESC>
void GraphicsBuffer<T, DESC>::lock(VOID **ppbData, unsigned long flags = 0, unsigned offsetToLock = 0, unsigned sizeToLock = 0) const
{
	if (!resource || resource->Lock(offsetToLock, sizeToLock, ppbData, flags) != D3D_OK)
		throw Exception(getLockFailedException());
}


template <class T, class DESC>
void GraphicsBuffer<T, DESC>::unlock() const
{
	if (!resource || resource->Unlock() != D3D_OK)
		throw Exception(getUnlockFailedException());
}


template <class T, class DESC> template <class TF>
void GraphicsBuffer<T, DESC>::setData(const std::vector<TF>& data, unsigned long flags) const
{
	setData(&data[0], data.size(), flags);
}


template <class T, class DESC> template <class TF, int N>
void GraphicsBuffer<T, DESC>::setData(TF (&data) [N], unsigned long flags) const
{
	setData(data, N, flags);
}


template <class T, class DESC> template <class TF>
void GraphicsBuffer<T, DESC>::setData(TF* data, int size, unsigned long flags) const
{
	void* p;

	int totalSize = sizeof(TF) * size;

	lock(&p, flags, 0, totalSize);

	memcpy(p, data, totalSize);

	unlock();
}


template <class T, class DESC> template <class TF>
void GraphicsBuffer<T, DESC>::getData(TF* data) const
{
	void* p;

	lock(&p, D3DLOCK_READONLY);

	memcpy(data, p, getDesc().Size);

	unlock();
}
