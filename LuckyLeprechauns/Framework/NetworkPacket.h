#pragma once

#define HEADER_SIZE 2

struct NetworkPacket
{
	NetworkPacket() {}
	NetworkPacket(unsigned char type, unsigned char size) : type(type), size(size) {}

	unsigned char type;
	unsigned char size;
	char data[256];

	static const unsigned getHeaderSize() { return HEADER_SIZE; }

	template <class T> const T& get(unsigned index = 0, unsigned* newIndex = 0) const;
	template <class T> void set(const T& value, unsigned index);
	template <class T> void add(const T& value);
	template <class T> void remove();
};


template <class T>
const T& NetworkPacket::get(unsigned index, unsigned* newIndex) const
{
	const T& value = *(const T*)&data[index];

	if (newIndex != 0)
		*newIndex = index + sizeof(T);

	return value;
}


template <class T>
void NetworkPacket::set(const T& value, unsigned index) 
{ 
	*(T*)&data[index] = value; 
}


template <class T>
void NetworkPacket::add(const T& value)
{
	set<T>(value, size);

	size += sizeof(T);
}


template <class T>
void NetworkPacket::remove()
{
	size -= sizeof(T);
}