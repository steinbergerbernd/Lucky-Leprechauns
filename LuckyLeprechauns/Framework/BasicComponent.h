#pragma once

#include "IBasicComponent.h"

template <class T>
class BasicComponent : public IBasicComponent
{
public:
	BasicComponent(T* manager) : manager(manager), enabled(true), local(true) {}
	virtual ~BasicComponent() = 0;

	virtual void init() {}
	virtual void update(const GameTime& gameTime) {}
	virtual void release() {}

	virtual void onLostDevice() {}
	virtual void onResetDevice(const GraphicsDevice& device) {}

	virtual bool isEnabled() const { return enabled; }
	virtual void setEnabled(bool enabled) { this->enabled = enabled; }

	virtual bool isLocal() const { return local; }
	virtual void setLocal(bool local) { this->local = local; }

protected:
	const T& getManager() const { return *manager; }
	T& getManager() { return *manager; }

private:
	T* manager;

	bool enabled;
	bool local;
};


template <class T>
BasicComponent<T>::~BasicComponent()
{
}