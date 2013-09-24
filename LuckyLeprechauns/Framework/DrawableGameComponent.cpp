#include "DrawableGameComponent.h"


DrawableGameComponent::DrawableGameComponent(Game* game) : GameComponent(game), scale(1.0f), position(0.0f)
{
}


DrawableGameComponent::~DrawableGameComponent()
{
}


Matrix DrawableGameComponent::getWorld() const
{
	return Matrix::createScale(scale) * Matrix::createFromQuaternion(rotation) * Matrix::createTranslation(position);
}


const Vector3& DrawableGameComponent::getPosition() const
{
	return position;
}


const Vector3& DrawableGameComponent::getScale() const
{
	return scale;
}


const Quaternion& DrawableGameComponent::getRotation() const
{
	return rotation;
}


const Vector3& DrawableGameComponent::getVelocity() const
{
	return velocity;
}


void DrawableGameComponent::setPosition(const Vector3& position)
{
	this->position = position;
}


void DrawableGameComponent::setScale(const Vector3& scale)
{
	this->scale = scale;
}


void DrawableGameComponent::setRotation(const Quaternion& rotation)
{
	this->rotation = rotation;
}


void DrawableGameComponent::setVelocity(const Vector3& velocity)
{
	this->velocity = velocity;
}


const Vector3 DrawableGameComponent::getForward() const
{
	return Vector3::transform(Vector3::forward, Matrix::createFromQuaternion(getRotation()));
}


const Vector3 DrawableGameComponent::getRight() const
{
	return Vector3::transform(Vector3::right, Matrix::createFromQuaternion(getRotation()));
}