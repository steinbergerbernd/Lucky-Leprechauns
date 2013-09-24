#include "ItemController.h"
#include "PlayerController.h"

ItemController::ItemController(LuckyLeprechauns* game) : LLDrawableGameComponent(game)
{
}


void ItemController::collectItem(unsigned itemType)
{
	((LuckyLeprechauns&)getManager()).getPlayer().getLeprechaun().collectItem(itemType);
	setEnabled(false);
}