#include "NetworkManager.h"


NetworkManager::NetworkManager(Game* game) : GameComponent(game), quit(false), thread(0)
{
}


NetworkManager::~NetworkManager()
{
}


void NetworkManager::init()
{
	BasicManager::init();
}


void NetworkManager::release()
{
	quit = true;

	if (thread)
	{
		::WaitForSingleObject(thread, INFINITE);
		::CloseHandle(thread);
	}

	BasicManager::release();
}


void NetworkManager::update(const GameTime& gameTime)
{
	BasicManager::update(gameTime);

	while (!inbox.empty())
	{
		process(inbox.front());
		inbox.pop();
	}
}


void NetworkManager::process(const NetworkPacket& packet)
{
}


void NetworkManager::onDisconnect()
{
}


void NetworkManager::send(const NetworkPacket& packet)
{
	outbox.push(packet);
}


void NetworkManager::start(const std::string& server, const std::string& port)
{
	this->server = server;
	this->port = port;

	thread = ::CreateThread(0, 0, NetworkManager::start, (void*)this, 0, 0);
}


DWORD WINAPI NetworkManager::start(LPVOID lpParam)
{
	HANDLE threads[2];
	unsigned long exitCodes[2];

	NetworkManager* manager = (NetworkManager*)lpParam;

	NetworkDevice device = manager->getDevice();

	while (!manager->quit)
	{
		// connect to server
		while (!device.isConnected() && !manager->quit)
			device.connect(manager->server, manager->port);

		if (device.isConnected())
		{
			// start send and receive threads
			threads[0] = ::CreateThread(0, 0, NetworkManager::send, lpParam, 0, 0);
			threads[1] = ::CreateThread(0, 0, NetworkManager::receive, lpParam, 0, 0);

			// monitor threads
			while (device.isConnected())
			{
				::Sleep(100);

				// check if threads are still alive
				for (unsigned i = 0; i < 2; ++i)
					::GetExitCodeThread(threads[i], &exitCodes[i]);

				// if quit flag is set or threads have finished
				if (manager->quit || exitCodes[0] != STILL_ACTIVE || exitCodes[1] != STILL_ACTIVE)
				{
					device.disconnect();

					::WaitForMultipleObjects(2, threads, true, INFINITE);

					for (unsigned i = 0; i < 2; ++i)
						::CloseHandle(threads[i]);

					manager->inbox.clear();
					manager->onDisconnect();
					manager->outbox.clear();
				}
			}
		}
	}

	return 0;
}


DWORD WINAPI NetworkManager::send(LPVOID lpParam)
{
	NetworkManager* manager = (NetworkManager*)lpParam;

	const NetworkDevice& device = manager->getDevice();
	NetworkPacketQueue& outbox = manager->outbox;

	try
	{
		while (device.isConnected())
		{
			while (!outbox.empty())
			{
				const NetworkPacket& packet = outbox.front();

				device.send((char*)&packet, packet.size + NetworkPacket::getHeaderSize());

				outbox.pop();
			}

			::Sleep(1);
		}
	}
	catch (const Exception& ex)
	{
		Console::out << ex.what() << Console::endl;
	}

	return 0;
}


DWORD WINAPI NetworkManager::receive(LPVOID lpParam)
{
	NetworkManager* manager = (NetworkManager*)lpParam;

	const NetworkDevice& device = manager->getDevice();
	NetworkPacketQueue& inbox = manager->inbox;

	NetworkPacket packet;
	int count;

	try
	{
		while (true)
		{
			if (device.receive((char*)&packet.type, 1) == 0)
				return 0;

			if (device.receive((char*)&packet.size, 1) == 0)
				return 0;

			unsigned received = 0;

			while (received < packet.size)
			{
				if ((count = device.receive(&packet.data[received], packet.size - received)) == 0)
					return 0;

				received += count;
			}

			manager->inbox.push(packet);
		}
	}
	catch (const Exception& ex)
	{
		Console::out << ex.what() << Console::endl;
	}

	return 0;
}