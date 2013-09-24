#pragma once

template <class T>
class ConstControlledComponent
{
public:
	ConstControlledComponent(const T* controller) : controller(controller) {}
	virtual ~ConstControlledComponent() = 0;

	const T& getController() const { return *controller; }

private:
	const T* controller;
};


template <class T>
ConstControlledComponent<T>::~ConstControlledComponent()
{
}