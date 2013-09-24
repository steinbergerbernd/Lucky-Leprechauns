#pragma once

#include "LLControlledNetworkComponent.h"
#include "DrawableGameComponent.h"

template <class T>
class DrawableNetwork : public LLControlledNetworkComponent<T>
{
public:
	DrawableNetwork(LLNetworkManager* manager, T* controller);

protected:
	void receive(const NetworkPacketDrawable& packet);
	void set(NetworkPacketDrawable& packet);
};


template <class T>
DrawableNetwork<T>::DrawableNetwork(LLNetworkManager* manager, T* controller) 
	: LLControlledNetworkComponent(manager, controller)
{
}


template <class T>
void DrawableNetwork<T>::receive(const NetworkPacketDrawable& packet)
{
	// TODO: prediction & interpolation

	T& controller = getController();

	controller.setPosition(packet.getPosition());
	controller.setRotation(packet.getRotation());
	controller.setScale(packet.getScale());
	controller.setVelocity(packet.getVelocity());
}


template <class T>
void DrawableNetwork<T>::set(NetworkPacketDrawable& packet)
{
	T& controller = getController();

	packet.setPosition(controller.getPosition());
	packet.setRotation(controller.getRotation());
	packet.setScale(controller.getScale());
	packet.setVelocity(controller.getVelocity());
}