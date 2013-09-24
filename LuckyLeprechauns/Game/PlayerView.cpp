#include "PlayerView.h"

PlayerView::PlayerView(LLGraphicsManager* manager, PlayerController* controller) : LLControlledGraphicsComponent(manager, controller)
{
	leprechaun = manager->addComponent<LeprechaunView>(manager, &controller->getLeprechaun());
	rainbow = manager->addComponent<RainbowView>(manager, &controller->getRainbow());
}


void PlayerView::setEnabled(bool enabled)
{
	leprechaun->setEnabled(enabled);
	rainbow->setEnabled(enabled);

	LLControlledGraphicsComponent::setEnabled(enabled);
}


void PlayerView::setLocal(bool local)
{
	leprechaun->setLocal(local);
	rainbow->setLocal(local);

	LLControlledGraphicsComponent::setLocal(local);
}