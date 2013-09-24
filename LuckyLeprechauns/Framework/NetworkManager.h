#pragma once

#include "GameComponent.h"
#include "BasicManager.h"

#include "Game.h"
#include "NetworkDevice.h"
#include "NetworkPacket.h"

class NetworkManager : public GameComponent, public BasicManager
{
	typedef Queue<NetworkPacket> NetworkPacketQueue;

public:
	NetworkManager(Game* game);
	virtual ~NetworkManager();

	virtual void init();
	virtual void release();
	virtual void update(const GameTime& gameTime);

	virtual void send(const NetworkPacket& packet);

	const NetworkDevice& getDevice() const { return device; }

protected:
	void start(const std::string& server, const std::string& port);

	virtual void process(const NetworkPacket& packet);
	virtual void onDisconnect();

private:
	HANDLE thread;

	static DWORD WINAPI start(LPVOID lpParam);

	static DWORD WINAPI send(LPVOID lpParam);
	static DWORD WINAPI receive(LPVOID lpParam);

	NetworkDevice device;

	NetworkPacketQueue outbox;
	NetworkPacketQueue inbox;

	std::string server;
	std::string port;

	bool quit;
};