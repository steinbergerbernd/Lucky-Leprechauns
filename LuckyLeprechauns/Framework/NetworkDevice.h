#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "STL.h"
#include "Converter.h"
#include "Exception.h"
#include "Constants.h"
#include "Helpers.h"

class NetworkDevice
{
public:
	NetworkDevice();
	~NetworkDevice();

	NetworkDevice(const NetworkDevice& other);
	NetworkDevice& operator=(const NetworkDevice& other);

	bool connect(const std::string& host, const std::string& port) const;
	void disconnect() const;

	int send(const std::vector<char>& buffer, int flags = 0) const;
	int send(const char* buffer, int length, int flags = 0) const;

	int receive(std::vector<char>& buffer, int flags = 0) const;
	int receive(char* buffer, int length, int flags = 0) const;

	bool isConnected() const;

private:
	static unsigned references;
	static SOCKET s;

	int validate(int result, const std::string& exceptionMessage) const;
};