#include "BasicManager.h"

#include <typeinfo>

BasicManager::BasicManager()
{
}


BasicManager::~BasicManager()
{
	for (ComponentCollection::iterator it = components.begin(); it != components.end(); ++it)
		delete *it;
}


void BasicManager::init()
{
	for (ComponentCollection::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->init();
}


void BasicManager::release()
{
	for (ComponentCollection::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->release();
}


void BasicManager::update(const GameTime& gameTime)
{
	for (ComponentCollection::iterator it = components.begin(); it != components.end(); ++it)
		if ((*it)->isEnabled())
			(*it)->update(gameTime);
}


void BasicManager::onResetDevice(const GraphicsDevice& device)
{
	for (ComponentCollection::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->onResetDevice(device);
}


void BasicManager::onLostDevice()
{
	for (ComponentCollection::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->onLostDevice();
}


void BasicManager::addPlayer(unsigned char playerId)
{
	remotePlayers[playerId] = remotePlayerPool.back();
	remotePlayers[playerId]->setEnabled(true);
	remotePlayerPool.pop_back();
}


void BasicManager::removePlayer(unsigned char playerId)
{
	remotePlayerPool.push_back(remotePlayers[playerId]);
	remotePlayers[playerId]->setEnabled(false);
	remotePlayers.erase(playerId);
}