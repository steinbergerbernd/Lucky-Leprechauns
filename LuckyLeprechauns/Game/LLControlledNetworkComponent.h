#pragma once

#include "ControlledNetworkComponent.h"
#include "LLNetworkComponent.h"

template <class T>
class LLControlledNetworkComponent : public ControlledNetworkComponent<T>
{
public:
	LLControlledNetworkComponent(LLNetworkManager* manager, T* controller);
	virtual ~LLControlledNetworkComponent() = 0;
};


template <class T>
LLControlledNetworkComponent<T>::LLControlledNetworkComponent(LLNetworkManager* manager, T* controller) 
	: ControlledNetworkComponent<T>(manager, controller)
{
}


template <class T>
LLControlledNetworkComponent<T>::~LLControlledNetworkComponent()
{
}