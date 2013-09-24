#include "LeprechaunController.h"

#include "PigController.h"
#include "PlayerController.h"

LeprechaunController::LeprechaunController(LuckyLeprechauns* game, PlayerController* player) 
	: LLDrawableGameComponent(game), items(3, ItemType::none), jumpPower(0), mushroomCount(0),
	dodgePower(0), movementPower(0), speedBoostFactor(0), detected(false), player(player)
{
	for (unsigned i = 0; i < TimerType::count; ++i)
	{
		timers[i] = configTimers[i] = 0;
		timerFinished[i] = false;
	}

	flags.reset();
	beginFlags.reset();

	hat = game->addComponent<HatController>(game, this, false, true);
	cane = game->addComponent<CaneController>(game, this, false, true);
}


void LeprechaunController::init()
{
	activeAttack = 0;

	setScale(Config::getValue<float>(LLConfigKeys::leprechaunScale));

	configTimers[TimerType::kick] = Config::getValue<float>(LLConfigKeys::leprechaunKickTime);
	configTimers[TimerType::hat] = Config::getValue<float>(LLConfigKeys::leprechaunHatTime);
	configTimers[TimerType::cane] = Config::getValue<float>(LLConfigKeys::leprechaunCaneTime);
	configTimers[TimerType::throwPig] = Config::getValue<float>(LLConfigKeys::leprechaunThrowPigTime);
	configTimers[TimerType::attackDouble] = Config::getValue<float>(LLConfigKeys::leprechaunAttackDoubleTime);
	configTimers[TimerType::invisibility] = Config::getValue<float>(LLConfigKeys::leprechaunInvisibilityTime);
	configTimers[TimerType::invulnerability] = Config::getValue<float>(LLConfigKeys::leprechaunInvulnerabilityTime);
	configTimers[TimerType::speedBoost] = Config::getValue<float>(LLConfigKeys::leprechaunSpeedBoostTime);
	configTimers[TimerType::showInvisibility] = Config::getValue<float>(LLConfigKeys::leprechaunShowInvisibilityTime);
	configTimers[TimerType::stoneHit] = Config::getValue<float>(LLConfigKeys::leprechaunStoneHitTime);

	jumpPower = Config::getValue<float>(LLConfigKeys::leprechaunJumpPower);
	dodgePower = Config::getValue<float>(LLConfigKeys::leprechaunDodgePower);
	movementPower = Config::getValue<float>(LLConfigKeys::leprechaunMovementPower);
	speedBoostFactor = Config::getValue<float>(LLConfigKeys::leprechaunSpeedBoostFactor);
	attackDoubleFactor = Config::getValue<float>(LLConfigKeys::leprechaunAttackDoubleFactor);
	stoneHitFactor = Config::getValue<float>(LLConfigKeys::leprechaunStoneHitFactor);
}


void LeprechaunController::update(const GameTime& gameTime)
{
	LuckyLeprechauns& game = (LuckyLeprechauns&)getManager();
	beginFlags.reset();

	setFlag(LeprechaunFlag::throwPig, false);
	setFlag(LeprechaunFlag::pigRobbery, false);
	setFlag(LeprechaunFlag::explosionTrap, false);
	usedItem = false;
	collectedItem = false;

	for (unsigned i = 0; i < TimerType::count; ++i)
		timerFinished[i] = timers[i] > 0 && (timers[i] -= gameTime.elapsed) <= 0;

	if (timerFinished[TimerType::caneHit])
		setFlag(LeprechaunFlag::caneHit, false);

	if (timerFinished[TimerType::stoneHit])
		setFlag(LeprechaunFlag::stoneHit, false);

	if (timerFinished[TimerType::attackDouble])
		setFlag(LeprechaunFlag::attackDouble, false);

	if (timerFinished[TimerType::invisibility])
		setFlag(LeprechaunFlag::invisibility, false);

	if (timerFinished[TimerType::invulnerability])
		setFlag(LeprechaunFlag::invulnerability, false);

	if (timerFinished[TimerType::showInvisibility])
		setFlag(LeprechaunFlag::showInvisibility, false);

	if (timerFinished[TimerType::speedBoost])
		setFlag(LeprechaunFlag::speedBoost, false);

	if (timerFinished[TimerType::kick])
		setBeginFlag(LeprechaunFlag::kick, true);

	Matrix rotation = Matrix::createFromQuaternion(getRotation());

	if (timerFinished[TimerType::cane])
	{
		setBeginFlag(LeprechaunFlag::cane, true);

		cane->setPosition(Vector3::transform(cane->getStartOffset(), rotation) + getPosition());
		cane->setRotation(cane->getStartRotation() * getRotation());
	}

	if (timerFinished[TimerType::hat])
	{
		setBeginFlag(LeprechaunFlag::hat, true);

		hat->setPosition(Vector3::transform(hat->getStartOffset(), rotation) + getPosition());
		hat->setRotation(hat->getStartRotation() * getRotation());
	}

	hat->setEnabled(getFlag(LeprechaunFlag::hat));
	cane->setEnabled(getFlag(LeprechaunFlag::cane));
}


void LeprechaunController::addMushroom()
{
	++mushroomCount;
}


void LeprechaunController::removeMushroom()
{
	--mushroomCount;
}


void LeprechaunController::collectItem(unsigned itemType)
{
	collectedItem = true;
	for (ItemSlots::iterator it = items.begin(); it != items.end(); ++it)
	{
		if (*it == ItemType::none)
		{
			(*it) = itemType;
			return;
		}
	}
}


void LeprechaunController::useItem(unsigned slot)
{
	if (items[slot] == ItemType::none || isStoneHit() || isCaneHit() || isFlying())
		return;

	switch(items[slot])
	{
		case ItemType::explosionTrap:
			{
				setBeginFlag(LeprechaunFlag::explosionTrap, true);
			}
			break;

		case ItemType::attackDouble:
			{
				setBeginFlag(LeprechaunFlag::attackDouble, true);
				timers[TimerType::attackDouble] = configTimers[TimerType::attackDouble];
			}
			break;

		case ItemType::invisibility:
			{
				if (isCarryingPig())
					return;

				setBeginFlag(LeprechaunFlag::invisibility, true);
				timers[TimerType::invisibility] = configTimers[TimerType::invisibility];
			}
			break;

		case ItemType::invulnerability:
			{
				setBeginFlag(LeprechaunFlag::invulnerability, true);
				timers[TimerType::invulnerability] = configTimers[TimerType::invulnerability];
			}
			break;

		case ItemType::pigRobbery:
			{
				if (isCarryingPig())
					return;

				setBeginFlag(LeprechaunFlag::pigRobbery, true);
			}
			break;

		case ItemType::showInvisibility:
			{
				setBeginFlag(LeprechaunFlag::showInvisibility, true);
				timers[TimerType::showInvisibility] = configTimers[TimerType::showInvisibility];
			}
			break;

		case ItemType::speedBoost:
			{
				setBeginFlag(LeprechaunFlag::speedBoost, true);
				timers[TimerType::speedBoost] = configTimers[TimerType::speedBoost];
			}
			break;
	}

	items[slot] = ItemType::none;
	usedItem = true;
}


void LeprechaunController::setMoving(const Vector3& movingDirection)
{
	if (!isCaneHit() && !isFlying())
	{
		this->movingDirection = movingDirection;
		setFlag(LeprechaunFlag::moving, movingDirection.lengthSquared() != 0);
	}
}


void LeprechaunController::kick()
{
	if (!isCaneHit() && !isFlying() && !isStoneHit())
	{
		timers[TimerType::kick] = configTimers[TimerType::kick];
		setBeginFlag(LeprechaunFlag::kickAnimation, true);
	}
}


void LeprechaunController::throwHat(float hatCharging)
{
	if (!hat->isEnabled() && !isCaneHit() && !isFlying() && !isStoneHit())
	{
		timers[TimerType::hat] = configTimers[TimerType::hat];
		this->hatCharging = MathHelper::saturate(hatCharging);
		setBeginFlag(LeprechaunFlag::hatAnimation, true);
	}
}


void LeprechaunController::throwCane()
{
	if (!cane->isEnabled() && !isCaneHit() && !isFlying() && !isStoneHit())
	{
		timers[TimerType::cane] = configTimers[TimerType::cane];
		setBeginFlag(LeprechaunFlag::caneAnimation, true);
	}
}


void LeprechaunController::throwPig()
{
	if (!isStoneHit())
	{
		setBeginFlag(LeprechaunFlag::throwPig, true);
		setBeginFlag(LeprechaunFlag::throwPigAnimation, true);
	}
}


void LeprechaunController::onKick(const Vector3& flyVelocity)
{
	setMoving(0);
	setBeginFlag(LeprechaunFlag::flying, true);
	setFlag(LeprechaunFlag::carryingPig, false);

	this->flyVelocity = flyVelocity;
}


void LeprechaunController::onCaneHit(float caneHitTime)
{
	setMoving(0);
	setBeginFlag(LeprechaunFlag::caneHit, true);
	setFlag(LeprechaunFlag::carryingPig, false);

	timers[TimerType::caneHit] = caneHitTime;
}


void LeprechaunController::onStoneHit()
{
	setBeginFlag(LeprechaunFlag::stoneHit, true);

	timers[TimerType::stoneHit] = configTimers[TimerType::stoneHit];
};


void LeprechaunController::onCapturePig()
{
	setBeginFlag(LeprechaunFlag::carryingPig, true);
	setInvisible(false);
}


void LeprechaunController::onDropPig()
{
	setFlag(LeprechaunFlag::carryingPig, false);
}


void LeprechaunController::jump()
{
	if (!isCaneHit() && !isFlying() && !isJumping() && !isDodgingLeft() && !isDodgingRight() && !isStoneHit() && !isCarryingPig())
		setBeginFlag(LeprechaunFlag::jumping, true);
}


void LeprechaunController::dodgeLeft()
{
	if (!isCaneHit() && !isStoneHit() && !isFlying() && !isJumping() && !isDodgingLeft() && !isDodgingRight() && !isCarryingPig())
		setBeginFlag(LeprechaunFlag::dodgingLeft, true);
}


void LeprechaunController::dodgeRight()
{
	if (!isCaneHit() && !isStoneHit() && !isFlying() && !isJumping() && !isDodgingLeft() && !isDodgingRight() && !isCarryingPig())
		setBeginFlag(LeprechaunFlag::dodgingRight, true);
}


void LeprechaunController::onLanding()
{
	setFlag(LeprechaunFlag::flying, false);
	setFlag(LeprechaunFlag::jumping, false);
	setFlag(LeprechaunFlag::dodgingLeft, false);
	setFlag(LeprechaunFlag::dodgingRight, false);
}


void LeprechaunController::setBeginFlags(const Flags& beginFlags)
{
	this->beginFlags |= beginFlags;
	this->flags |= beginFlags;
}


void LeprechaunController::setBeginFlag(unsigned flag, bool value)
{
	beginFlags[flag] = value;

	if (value)
		flags[flag] = true;
}


void LeprechaunController::setLocal(bool local)
{
	hat->setLocal(local);
	cane->setLocal(local);
	LLDrawableGameComponent::setLocal(local);
}


void LeprechaunController::onCaneEnd()
{
	setFlag(LeprechaunFlag::cane, false);
}


void LeprechaunController::onHatEnd()
{
	setFlag(LeprechaunFlag::hat, false);
}


float LeprechaunController::getMovementPower() const
{
	float movementPower = this->movementPower;

	if (hasSpeedBoost())
		movementPower *= speedBoostFactor;

	if (isStoneHit())
		movementPower /= stoneHitFactor;

	 return movementPower;
}


unsigned char LeprechaunController::getPlayerId() const
{
	return player->getPlayerId();
}


const Vector3& LeprechaunController::getPlayerColor() const
{
	return player->getPlayerColor();
}
