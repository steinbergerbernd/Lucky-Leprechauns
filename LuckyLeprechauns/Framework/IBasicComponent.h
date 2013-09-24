#pragma once

#define WIN32_LEAN_AND_MEAN

#include "GameTime.h"
#include "GraphicsDevice.h"

class IBasicComponent
{
public:
	IBasicComponent();
	virtual ~IBasicComponent() = 0;

	IBasicComponent(const IBasicComponent& other);
	void operator=(const IBasicComponent& other);

	virtual void init() = 0;
	virtual void release() = 0;
	virtual void update(const GameTime& gameTime) = 0;

	virtual void onLostDevice() = 0;
	virtual void onResetDevice(const GraphicsDevice& device) = 0;

	virtual bool isEnabled() const = 0;
	virtual void setEnabled(bool enabled) = 0;
};