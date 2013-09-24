#include "GuiSystem.h"
#include "Game.h"

GuiSystem::GuiSystem()
{
}


GuiSystem::~GuiSystem(void)
{
}

float GuiSystem::GetElapsedTime()
{
	return Game::getGameTime().total;
}