#pragma once

#include "BasicComponent.h"
#include "NetworkManager.h"

class NetworkComponent : public BasicComponent<NetworkManager>
{
public:
	NetworkComponent(NetworkManager* manager);
	virtual ~NetworkComponent() = 0;
};