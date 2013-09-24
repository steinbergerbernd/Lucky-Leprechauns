#include "LeprechaunNetwork.h"
#include "PlayerController.h"

LeprechaunNetwork::LeprechaunNetwork(LLNetworkManager* manager, LeprechaunController* controller)
	: DrawableNetwork(manager, controller)
{
	hat = manager->addComponent<HatNetwork>(manager, &controller->getHat());
	cane = manager->addComponent<CaneNetwork>(manager, &controller->getCane());
}


void LeprechaunNetwork::update(const GameTime& gameTime)
{
	LeprechaunController& leprechaun = getController();

	if (!isLocal() || !leprechaun.isEnabled())
		return;

	NetworkPacketLeprechaun packet(leprechaun.getPlayerId());

	DrawableNetwork::set(packet);

	packet.add<LeprechaunController::Flags>(leprechaun.getFlags());
	packet.add<LeprechaunController::Flags>(leprechaun.getBeginFlags());

	packet.add<unsigned char>((unsigned char)leprechaun.getPlayer().getRainbow().getProgressInt());

	getManager().send(packet);
}


void LeprechaunNetwork::receive(const NetworkPacketDrawable& packet)
{
	LeprechaunController& leprechaun = getController();

	DrawableNetwork::receive(packet);

	unsigned index = NetworkPacketLeprechaun::getOffset();

	leprechaun.setFlags(packet.get<LeprechaunController::Flags>(index, &index));
	leprechaun.setBeginFlags(packet.get<LeprechaunController::Flags>(index, &index));

	leprechaun.getPlayer().getRainbow().setProgress((float)packet.get<unsigned char>(index, &index));
}


void LeprechaunNetwork::onKick(const NetworkPacket& packet)
{
	getController().onKick(packet.get<Vector3>());
}


void LeprechaunNetwork::onCaneHit(const NetworkPacket& packet)
{
	getController().onCaneHit(packet.get<float>());
}


void LeprechaunNetwork::onCapturePig()
{
	getController().onCapturePig();
}

void LeprechaunNetwork::onDropPig()
{
	getController().onDropPig();
}


void LeprechaunNetwork::onStoneHit(const NetworkPacket& packet)
{
	getController().onStoneHit();
}


void LeprechaunNetwork::setLocal(bool local)
{
	hat->setLocal(local);
	cane->setLocal(local);

	LLControlledNetworkComponent::setLocal(local);
}


void LeprechaunNetwork::setEnabled(bool enabled)
{
	hat->setEnabled(enabled);
	cane->setEnabled(enabled);

	LLControlledNetworkComponent::setEnabled(enabled);
}
