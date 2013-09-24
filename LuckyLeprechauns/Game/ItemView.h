#pragma once

#include "LLGraphicsComponent.h"
#include "ItemController.h"

class ItemView : public LLGraphicsComponent
{
	typedef std::list<ItemController*> ItemCollection;
	typedef std::vector<Matrix> InstanceCollection;

public:
	ItemView(LLGraphicsManager* manager, const ItemCollection* items);

	void init();
	void draw(const GameTime& gameTime);

private:
	ItemCollection items;

	FBXModel model;

	InstanceCollection instances;
};

