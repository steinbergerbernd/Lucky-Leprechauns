#include "RainbowController.h"
#include "PlayerController.h"

RainbowController::RainbowController(LuckyLeprechauns* game, PlayerController* player) 
	: LLDrawableGameComponent(game), player(player), progress(0)
{
}


void RainbowController::init()
{
	goldpotRadius = Config::getValue<float>(LLConfigKeys::goldpotRadius);
	goldpotScale = Config::getValue<float>(LLConfigKeys::goldpotScale);
	goldpotPosition = Config::getValue<Vector3>(LLConfigKeys::goldpotPosition);
}


unsigned char RainbowController::getPlayerId() const
{
	return player->getPlayerId();
}


void RainbowController::setPosition(const Vector3& position)
{
	LLDrawableGameComponent::setPosition(position);

	Vector3 distance = goldpotPosition - position;
	Vector3 direction = Vector3::normalize(Vector3(distance.x, 0, distance.z));

	float sign = Vector3::cross(Vector3::forward, direction).y > 0 ? 1.0f : -1.0f;

	float angle = std::acos(MathHelper::clamp(Vector3::dot(Vector3::forward, direction), 0.0f, 1.0f));

	setScale(distance.length() + goldpotRadius * goldpotScale);
	setRotation(Quaternion::createFromAxisAngle(Vector3::up, angle));
}


const Vector3& RainbowController::getPlayerColor() const
{
	return player->getPlayerColor();
}