#pragma once
#include "GuiComponent.h"
#include "TinyXML\tinyxml.h"
#include <map>
#include <vector>

class LLGuiManager;

class HudGuiComponent : public GuiComponent
{
	struct MinimapElement
	{
		std::string id;
	};

	struct DynamicMinimapElement : MinimapElement
	{
		bool isVisible;
	};

	struct LeprechaunMinimapElement : DynamicMinimapElement
	{
		unsigned playerId;
	};

	struct StaticMinimapElement : MinimapElement
	{
		float x;
		float y;
	};

public:
	HudGuiComponent(LLGuiManager* guiManager);

	void init();
	void update(const GameTime& gameTime);

	void onResetDevice(const GraphicsDevice& device);

	void setWinner(unsigned char winner);
private:
	int activeAttack;
	std::string attackSlotId;
	std::string itemSlotId;

	float minimapScaleWidth;
	float minimapScaleHeight;
	int minimapImgSize;
	int minimapWidth;
	int minimapHeight;
	Vector2 minimapZeroPoint;

	std::vector<StaticMinimapElement> staticElements;
	std::vector<LeprechaunMinimapElement> leprechaunElements;
	DynamicMinimapElement pigElement;

	std::vector<int> mushroomOwners;
	std::map<std::string, std::string> progressMap;
	std::map<std::string, std::string> mushroomCountMap;

	typedef std::map<unsigned, std::string> NickMap;
	typedef std::map<unsigned, std::string> ClassMap;
	typedef std::map<unsigned, unsigned> ItemBuffer;

	ClassMap attackMap;
	ClassMap itemMap;

	ItemBuffer itemBuffer;

	NickMap nicknames;

	std::string getMushroomId(int owner, int mushroom) const;

	void resetMushrooms();

	void readSlotElements(ClassMap& map, TiXmlElement* root, std::string id);

	void setPosition(const DynamicMinimapElement& element, float x, float y);
	void setPosition(const std::string& elementId, float x, float y);
	void setVisibility(DynamicMinimapElement& element, bool visible);
	void setVisibility(const std::string& elementId, bool visible);

	void setMinimapImg(const std::string& id, float x, float y, bool saveAsStatic = false);
};

