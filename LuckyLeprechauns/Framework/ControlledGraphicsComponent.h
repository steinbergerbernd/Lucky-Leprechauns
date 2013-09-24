#pragma once

#include "GraphicsComponent.h"
#include "ConstControlledComponent.h"

template <class T>
class ControlledGraphicsComponent : public GraphicsComponent, public ConstControlledComponent<T>
{
public:
	ControlledGraphicsComponent(GraphicsManager* graphicsManager, const T* controller);
	virtual ~ControlledGraphicsComponent() = 0;
};

template <class T>
ControlledGraphicsComponent<T>::ControlledGraphicsComponent(GraphicsManager* graphicsManager, const T* controller) 
	: GraphicsComponent(graphicsManager), ConstControlledComponent(controller)
{
}


template <class T>
ControlledGraphicsComponent<T>::~ControlledGraphicsComponent()
{
}