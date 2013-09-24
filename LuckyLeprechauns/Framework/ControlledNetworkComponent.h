#pragma once

#include "NetworkComponent.h"
#include "ControlledComponent.h"

template <class T>
class ControlledNetworkComponent : public NetworkComponent, public ControlledComponent<T>
{
public:
	ControlledNetworkComponent(NetworkManager* manager, T* controller);
	virtual ~ControlledNetworkComponent() = 0;
};


template <class T>
ControlledNetworkComponent<T>::ControlledNetworkComponent(NetworkManager* manager, T* controller)
	: NetworkComponent(manager), ControlledComponent(controller)
{
}


template <class T>
ControlledNetworkComponent<T>::~ControlledNetworkComponent()
{
}