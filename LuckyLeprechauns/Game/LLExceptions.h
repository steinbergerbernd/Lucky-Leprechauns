#pragma once

#include "Exceptions.h"

struct LLExceptions : public Exceptions
{
	static const char* invalidNetworkPacketType;
};