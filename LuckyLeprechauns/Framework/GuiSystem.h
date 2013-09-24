#pragma once

#include "Rocket/Core/SystemInterface.h"

class GuiSystem : public Rocket::Core::SystemInterface
{
public:
	GuiSystem();
	~GuiSystem(void);

	virtual float GetElapsedTime();
};

