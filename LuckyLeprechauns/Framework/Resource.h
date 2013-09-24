#pragma once

template <class T>
class Resource
{
public:
	Resource();
	Resource(T* resource);
	Resource(const Resource& other);
	virtual ~Resource() = 0;

	void addRef();
	void release();

	Resource& operator=(const Resource& other);

	T** operator&();

	bool operator==(const T* other) const;
	bool operator!=(const T* other) const;

	bool operator==(const Resource& other) const;
	bool operator!=(const Resource& other) const;

	operator bool() const { return resource != 0; }
	operator T*() const { return resource; }
	//operator T**() { return &resource; }

	operator const T*() const { return resource; }

//protected:
	T* resource;
};


template <class T>
Resource<T>::Resource() : resource(0)
{	
}


template <class T>
Resource<T>::Resource(T* resource) : resource(resource)
{
	if (resource)
		resource->AddRef();
}


template <class T>
Resource<T>::Resource(const Resource<T>& other)
{
	resource = other.resource;

	if (resource)
		resource->AddRef();
}


template <class T>
Resource<T>::~Resource()
{
	if (resource)
		resource->Release();
}


template <class T>
void Resource<T>::release()
{
	if (resource)
		resource->Release();

	resource = 0;
}


template <class T>
void Resource<T>::addRef()
{
	if (resource)
		resource->AddRef();
}


template <class T>
Resource<T>& Resource<T>::operator=(const Resource<T>& other)
{
	if (resource != other.resource)
	{
		if (resource)
			resource->Release();

		if (other.resource)
			other.resource->AddRef();

		resource = other.resource;
	}

	return *this;
}


template <class T>
T** Resource<T>::operator&()
{
	return &resource;
}


template <class T>
bool Resource<T>::operator==(const Resource& other) const
{
	return resource == other.resource;
}


template <class T>
bool Resource<T>::operator!=(const Resource& other) const
{
	return !(*this == other);
}


template <class T>
bool Resource<T>::operator==(const T* other) const
{
	return resource == other;
}


template <class T>
bool Resource<T>::operator!=(const T* other) const
{
	return !(resource == other);
}