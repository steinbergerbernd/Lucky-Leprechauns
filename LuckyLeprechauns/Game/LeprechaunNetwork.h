#pragma once

#include "DrawableNetwork.h"
#include "LeprechaunController.h"

#include "HatNetwork.h"
#include "CaneNetwork.h"

class LeprechaunNetwork : public DrawableNetwork<LeprechaunController>
{
public:
	LeprechaunNetwork(LLNetworkManager* manager, LeprechaunController* controller);

	void update(const GameTime& gameTime);
	void receive(const NetworkPacketDrawable& packet);

	void setLocal(bool local);
	void setEnabled(bool enabled);

	void onKick(const NetworkPacket& packet);
	void onCaneHit(const NetworkPacket& packet);
	void onCapturePig();
	void onStoneHit(const NetworkPacket& packet);
	void onDropPig();

	CaneNetwork& getCane() { return *cane; }
	HatNetwork& getHat() { return *hat; }

private:
	HatNetwork* hat;
	CaneNetwork* cane;
};
