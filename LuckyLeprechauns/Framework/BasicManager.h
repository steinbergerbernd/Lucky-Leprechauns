#pragma once

#define WIN32_LEAN_AND_MEAN

#include "STL.h"
#include "Types.h"
#include "Helpers.h"
#include "Constants.h"

#include "GraphicsDevice.h"
#include "NetworkDevice.h"
#include "IBasicComponent.h"

class BasicManager
{
public:
	typedef std::list<IBasicComponent*> ComponentCollection;
	typedef std::map<unsigned char, IBasicComponent*> ComponentMap;

	BasicManager();
	virtual ~BasicManager() = 0;

	BasicManager(const BasicManager& other);
	void operator=(const BasicManager& other);

	virtual void init();
	virtual void update(const GameTime& gameTime);
	virtual void release();

	virtual void onLostDevice();
	virtual void onResetDevice(const GraphicsDevice& device);

	virtual void addPlayer(unsigned char playerId);
	virtual void removePlayer(unsigned char playerId);

	template <class T, class U>
	T* addComponent(U* manager, bool enabled = true, bool local = true);

	template <class T, class U, class V>
	T* addComponent(U* manager, V* param, bool enabled = true, bool local = true);

	const ComponentMap& getRemotePlayers() const { return remotePlayers; }

protected:
	ComponentCollection components;

	ComponentCollection remotePlayerPool;
	ComponentMap remotePlayers;
};


template <class T, class U>
T* BasicManager::addComponent(U* manager, bool enabled, bool local)
{
	T* component = new T(manager);

	component->setEnabled(enabled);
	component->setLocal(local);

	components.push_back(component);

	return component;
}


template <class T, class U, class V>
T* BasicManager::addComponent(U* manager, V* param, bool enabled, bool local)
{
	T* component = new T(manager, param);

	component->setEnabled(enabled);
	component->setLocal(local);

	components.push_back(component);

	return component;
}