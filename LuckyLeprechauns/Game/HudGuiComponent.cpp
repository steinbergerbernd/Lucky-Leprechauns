#include "HudGuiComponent.h"

#include "LLGuiManager.h"
#include "LLConfigKeys.h"
#include "LuckyLeprechauns.h"
#include "PlayerController.h"
#include "LeprechaunController.h"
#include "PigController.h"
#include "MushroomController.h"

HudGuiComponent::HudGuiComponent(LLGuiManager* guiManager) : GuiComponent(guiManager), activeAttack(-1)
{
}

void HudGuiComponent::init()
{
	GuiComponent::init();

	mushroomOwners = std::vector<int>(Config::getValue<unsigned>(LLConfigKeys::gameMushroomCount), -1);

	std::string configPath = Config::getValue(LLConfigKeys::guiConfigPath);

	TiXmlDocument doc;
	doc.LoadFile(configPath.c_str());
	TiXmlElement* root = doc.FirstChildElement("Gui");
	TiXmlElement* hud = root->FirstChildElement("Hud");

	// load the RML document for libRocket
	loadDocument(hud->Attribute("path"));
	
	// read all attacks
	TiXmlElement* attacks = hud->FirstChildElement("Attacks");
	attackSlotId = attacks->Attribute("elementId");
	readSlotElements(attackMap, attacks, "Attack");

	// read all items
	TiXmlElement* items = hud->FirstChildElement("Items");
	itemSlotId = items->Attribute("elementId");
	readSlotElements(itemMap, items, "Item");

	TiXmlElement* minimap = root->FirstChildElement("Minimap");
	std::string minimapId = minimap->Attribute("elementId");

	// read all configuration variables for the minimap
	minimapImgSize = Converter::convert<int>(minimap->Attribute("imageSize"));
	minimapWidth = Converter::convert<int>(getElementProperty(minimapId, "width"));
	minimapHeight = Converter::convert<int>(getElementProperty(minimapId, "height"));
	minimapScaleWidth = (float)minimapWidth / Converter::convert<float>(minimap->Attribute("inGameWidth"));
	minimapScaleHeight = (float)minimapHeight / Converter::convert<float>(minimap->Attribute("inGameHeight"));
	minimapZeroPoint = Vector2((float)(minimapWidth / 2), (float)(minimapHeight / 2));

	TiXmlElement* leprechauns = minimap->FirstChildElement("Leprechauns");
	TiXmlElement* leprechaun = leprechauns->FirstChildElement("Leprechaun");

	// read the leprechauns
	while(leprechaun)
	{
		LeprechaunMinimapElement leprechaunElement;
		leprechaunElement.id = leprechaun->Attribute("elementId");
		leprechaunElement.playerId = Converter::convert<unsigned>(leprechaun->Attribute("playerId"));
		nicknames[leprechaunElement.playerId] = leprechaun->Attribute("nickname");
		setVisibility(leprechaunElement, true);

		leprechaunElements.push_back(leprechaunElement);
		leprechaun = leprechaun->NextSiblingElement();
	}

	// read the pig
	TiXmlElement* pig = minimap->FirstChildElement("Pig");
	pigElement.id = pig->Attribute("elementId");
	setVisibility(pigElement, false);

	resetMushrooms();
}

void HudGuiComponent::resetMushrooms()
{
	for(int i = 0; i < (int)mushroomOwners.size(); ++i)
		setVisibility(getMushroomId(mushroomOwners[i], i), true);
}

void HudGuiComponent::readSlotElements(ClassMap& map, TiXmlElement* root, std::string id)
{
	TiXmlElement* element = root->FirstChildElement(id.c_str());
	while(element)
	{
		unsigned id = Converter::convert<unsigned>(element->Attribute("id"));
		std::string className = element->Attribute("class");
		map[id] = className;
		element = element->NextSiblingElement();
	}
}

void HudGuiComponent::setMinimapImg(const std::string& elementId, float x, float y, bool saveAsStatic)
{
	// the coordinates are multiplied by the scale factor and translated to the center of the minimap
	// then the half size of the image is subtracted so the center of the image is in the center of the minimap
	int minimapLeft = getManager().getGraphicsDevice().getViewport().Width - minimapWidth;
	int left = (int)(x * minimapScaleWidth + minimapZeroPoint.x - minimapImgSize / 2.0f + minimapLeft);
	int top = (int)(y * minimapScaleHeight + minimapZeroPoint.y - minimapImgSize / 2.0f + 10);
	setElementProperty(elementId, "left", Converter::convert<std::string>(left));
	setElementProperty(elementId, "top", Converter::convert<std::string>(top));

	// if it is a static object and doesn't move on the minimap then save it for restoring it in the onResetDevice method
	if(saveAsStatic)
	{
		StaticMinimapElement staticElement;
		staticElement.id = elementId;
		staticElement.x = x;
		staticElement.y = y;
		staticElements.push_back(staticElement);
		setVisibility(elementId, true);
	}
}

void HudGuiComponent::update(const GameTime& gameTime)
{
	GuiComponent::update(gameTime);

	LLGuiManager& manager =  (LLGuiManager&)getManager();
	const PlayerController& localPlayer = manager.getLocalPlayer();
	const LeprechaunController& localLeprechaun = localPlayer.getLeprechaun();

	if (localLeprechaun.getAttack() != activeAttack)
	{
		activeAttack = localLeprechaun.getAttack();
		setElementClass(attackSlotId, attackMap[activeAttack]);
	}

	const LeprechaunController::ItemSlots& items = localLeprechaun.getItems();

	for (unsigned i = 0; i < items.size(); ++i)
	{
		ItemBuffer::iterator it = itemBuffer.find(i);

		if (it == itemBuffer.end() || it->second != items[i])
		{
			setElementClass(itemSlotId + Converter::convert<std::string>(i), itemMap[items[i]]);
			itemBuffer[i] = items[i];
		}
	}

	const LuckyLeprechauns::ComponentMap& remotePlayers = manager.getRemotePlayers();

	for(std::vector<LeprechaunMinimapElement>::iterator it = leprechaunElements.begin(); it != leprechaunElements.end(); ++it)
	{
		unsigned char playerId = it->playerId;
		std::string strPlayerId = Converter::convert<std::string>(it->playerId);

		LuckyLeprechauns::ComponentMap::const_iterator remotePlayersIt = remotePlayers.find(playerId);

		if (remotePlayersIt != remotePlayers.end() || localPlayer.getPlayerId() == playerId)
		{
			const PlayerController& player = localPlayer.getPlayerId() == playerId ? localPlayer : *(const PlayerController*)remotePlayersIt->second;
			const LeprechaunController& leprechaun = player.getLeprechaun();
			const Vector3& position = leprechaun.getPosition();

			std::string mushroomCountId = "mushroom_count" + strPlayerId;
			std::string progressId = "rainbow_progress" + strPlayerId;

			std::string mushroomCount = Converter::convert<std::string>(leprechaun.getMushroomCount());
			std::string progress = Converter::convert<std::string>(player.getRainbow().getProgressInt());

			if (mushroomCount != mushroomCountMap[mushroomCountId])
			{
				mushroomCountMap[mushroomCountId] = mushroomCount;
				setElementText(mushroomCountId, mushroomCount);
			}

			if (progress != progressMap[progressId])
			{
				progressMap[progressId] = progress;
				setElementText(progressId, progress + "%");
			}

			setPosition(*it, position.x, position.z);

			if (leprechaun.isLocal() && !(*it).isVisible)
				setVisibility(*it, true);
			else if (!leprechaun.isLocal() && (*it).isVisible == (leprechaun.hasInvisibility() && !leprechaun.isDetected()))
				setVisibility(*it, !(*it).isVisible);
		}
		else if((*it).isVisible)
		{
			setVisibility(*it, false);
			setElementText("mushroom_count" + strPlayerId,	"0");
			setElementText("rainbow_progress" + strPlayerId, "0%");
		}
	}

	const PigController& pig = manager.getPig();

	if(pig.isEnabled())
	{
		if(!pigElement.isVisible)
			setVisibility(pigElement, true);

		setPosition(pigElement, pig.getPosition().x, pig.getPosition().z);
	}
	else if(pigElement.isVisible)
		setVisibility(pigElement, false);

	const LuckyLeprechauns::MushroomCollection& mushrooms = manager.getMushrooms();
	unsigned i = 0;

	for (LuckyLeprechauns::MushroomCollection::const_iterator it = mushrooms.begin(); it != mushrooms.end(); ++it)
	{
		const LeprechaunController* leprechaun = (*it)->getOwner();
		const Vector3& position = (*it)->getPosition();

		int owner = leprechaun == 0 ? -1 : leprechaun->getPlayerId();
		
		std::string mushroomId = getMushroomId(owner, i);

		if (mushroomOwners[i] != owner)
		{
			setVisibility(getMushroomId(mushroomOwners[i], i), false);
			mushroomOwners[i] = owner;
			setVisibility(mushroomId, true);
		}

		setPosition(mushroomId, position.x, position.z);

		++i;
	}
}

void HudGuiComponent::setWinner(unsigned char winner)
{
	setElementClass("body", "gray");
	setElementText("gameWon", nicknames[winner] + " Player Won!");
}

std::string HudGuiComponent::getMushroomId(int owner, int mushroom) const
{
	return "mm_mushroom_" + (owner == -1 ? "neutral" : ("player" + Converter::convert<std::string>(owner))) +
		"_" + Converter::convert<std::string>(mushroom);
}

void HudGuiComponent::onResetDevice(const GraphicsDevice& device)
{
	GuiComponent::onResetDevice(device);

	// restore the attack
	setElementClass(attackSlotId, attackMap[activeAttack]);

	// restore the static elements
	for(std::vector<StaticMinimapElement>::iterator it = staticElements.begin(); it != staticElements.end(); ++it)
	{
		setMinimapImg(it->id, it->x, it->y);
		setVisibility(it->id, true);
	}

	for(std::vector<LeprechaunMinimapElement>::iterator it = leprechaunElements.begin(); it != leprechaunElements.end(); ++it)
		setVisibility(*it, (*it).isVisible);
	setVisibility(pigElement, false);

	itemBuffer.clear();

	resetMushrooms();
}

void HudGuiComponent::setPosition(const DynamicMinimapElement& element, float x, float y)
{
	setPosition(element.id, x, y);
}

void HudGuiComponent::setPosition(const std::string& elementId, float x, float y)
{
	setMinimapImg(elementId, -x, y);
}

void HudGuiComponent::setVisibility(DynamicMinimapElement& element, bool visible)
{
	setVisibility(element.id, visible);
	element.isVisible = visible;
}

void HudGuiComponent::setVisibility(const std::string& elementId, bool visible)
{
	setElementProperty(elementId, "visibility", visible ? "visible" : "hidden");
}
