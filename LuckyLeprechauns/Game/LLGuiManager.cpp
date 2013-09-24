#include "LLGuiManager.h"

#include "HudGuiComponent.h"
#include "LLConfigKeys.h"

#include "HudGuiComponent.h"

#include "TinyXML\tinyxml.h"


LLGuiManager::LLGuiManager(LuckyLeprechauns* game) : GuiManager(game), lastUpdate(0.0f)
{
	hud = addComponent<HudGuiComponent>(this);
}


void LLGuiManager::update(const GameTime& gameTime)
{
	GuiManager::update(gameTime);
}

void LLGuiManager::setWinner(unsigned char winner)
{
	hud->setWinner(winner);
}

void LLGuiManager::loadFonts()
{
	GuiManager::loadFonts();

	std::string configPath = Config::getValue(LLConfigKeys::guiConfigPath);

	TiXmlDocument doc;

	doc.LoadFile(configPath.c_str());
	
	TiXmlElement* root = doc.FirstChildElement("Gui");
	TiXmlElement* fonts = root->FirstChildElement("Fonts");
	TiXmlElement* font = fonts->FirstChildElement("Font");

	std::string fontsPath = fonts->Attribute("path");

	while(font)
	{
		std::string fileName = font->Attribute("file");
		loadFont(fontsPath + fileName);
		font = font->NextSiblingElement();
	}
}