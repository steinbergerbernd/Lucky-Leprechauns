#pragma once

#include "ControlledGraphicsComponent.h"
#include "LLGraphicsComponent.h"

template <class T>
class LLControlledGraphicsComponent : public ControlledGraphicsComponent<T>
{
public:
	LLControlledGraphicsComponent(GraphicsManager* graphicsManager, const T* controller);
	virtual ~LLControlledGraphicsComponent() = 0;
};


template <class T>
LLControlledGraphicsComponent<T>::LLControlledGraphicsComponent(GraphicsManager* graphicsManager, const T* controller) 
	: ControlledGraphicsComponent<T>(graphicsManager, controller)
{
}


template <class T>
LLControlledGraphicsComponent<T>::~LLControlledGraphicsComponent()
{
}