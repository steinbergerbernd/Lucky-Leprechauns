#pragma once
#include "LLDrawableGameComponent.h"

class ItemController : public LLDrawableGameComponent
{
public:
	ItemController(LuckyLeprechauns* game);

	void collectItem(unsigned itemType);
};

