#pragma once

template <class T>
class ControlledComponent
{
public:
	ControlledComponent(T* controller) : controller(controller) {}
	virtual ~ControlledComponent() = 0;

	T& getController() { return *controller; }
	const T& getController() const { return *controller; }

private:
	T* controller;
};


template <class T>
ControlledComponent<T>::~ControlledComponent()
{
}