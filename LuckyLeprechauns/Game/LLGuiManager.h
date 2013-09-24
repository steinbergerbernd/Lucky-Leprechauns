#pragma once

#include "GuiManager.h"
#include "PlayerController.h"
#include "PigController.h"

class HudGuiComponent;

class LLGuiManager : public GuiManager
{
public:
	LLGuiManager(LuckyLeprechauns* game);

	void update(const GameTime& game);
	void loadFonts();

	const PlayerController& getLocalPlayer() { return ((LuckyLeprechauns&)getManager()).getPlayer(); }
	const ComponentMap& getRemotePlayers() { return getManager().getRemotePlayers(); }
	const PigController& getPig() { return ((LuckyLeprechauns&)getManager()).getPig(); }
	const LuckyLeprechauns::MushroomCollection& getMushrooms() { return ((LuckyLeprechauns&)getManager()).getMushrooms(); }

	void setWinner(unsigned char winner);
private:
	HudGuiComponent* hud;
	float lastUpdate;
};

