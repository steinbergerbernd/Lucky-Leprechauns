#pragma once

#include "NetworkComponent.h"
#include "LLNetworkManager.h"

class LLNetworkComponent : public NetworkComponent
{
public:
	LLNetworkComponent(LLNetworkManager* manager);
	virtual ~LLNetworkComponent() = 0;
};