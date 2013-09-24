#include "NetworkDevice.h"

unsigned NetworkDevice::references = 0;
SOCKET NetworkDevice::s = INVALID_SOCKET;


NetworkDevice::NetworkDevice()
{
	if (++references == 1)
	{
		WSADATA wsaData;

		if (::WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
			throw Exception(Exceptions::networkFailed);
	}
}


NetworkDevice::~NetworkDevice()
{
	if (--references == 0)
	{
		if (s != INVALID_SOCKET)
			::closesocket(s);

		::WSACleanup();
	}
}


NetworkDevice::NetworkDevice(const NetworkDevice& other)
{
	s = other.s;
	references++;
}


NetworkDevice& NetworkDevice::operator=(const NetworkDevice& other)
{
	s = other.s;
	references++;

	return *this;
}


int NetworkDevice::validate(int result, const std::string& exceptionMessage) const
{
	if (result == SOCKET_ERROR)
	{
		s = INVALID_SOCKET;

		Console::out << "Windows Socket Error: " << ::WSAGetLastError() << Console::endl;

		throw Exception(exceptionMessage);
	}

	return result;
}


bool NetworkDevice::connect(const std::string& host, const std::string& port) const
{
	::addrinfo hints;
	::addrinfo* result;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (::getaddrinfo(host.c_str(), port.c_str(), &hints, &result) != 0)
		throw Exception(Exceptions::networkFailed);

	if ((s = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET)
		throw Exception(Exceptions::networkFailed);

	if (::connect(s, result->ai_addr, (int)(result->ai_addrlen)) == SOCKET_ERROR)
		s = INVALID_SOCKET;

	return s != INVALID_SOCKET;
}


void NetworkDevice::disconnect() const
{
	::closesocket(s);
	s = INVALID_SOCKET;
}


int NetworkDevice::send(const std::vector<char>& buffer, int flags) const
{
	return send(&buffer[0], (int)buffer.size(), flags);
}


int NetworkDevice::send(const char* buffer, int length, int flags) const
{
	return validate(::send(s, buffer, length, flags), Exceptions::networkFailed);
}


int NetworkDevice::receive(std::vector<char>& buffer, int flags) const
{
	return receive(&buffer[0], (int)buffer.size(), flags);
}


int NetworkDevice::receive(char* buffer, int length, int flags) const
{
	return validate(::recv(s, buffer, length, flags), Exceptions::networkFailed);
}


bool NetworkDevice::isConnected() const
{
	return s != INVALID_SOCKET;
}