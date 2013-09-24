#pragma once

#include "GameComponent.h"
#include "BasicManager.h"

#include "Game.h"
#include "Rocket\Core.h"
#include "GuiSystem.h"
#include "GuiRenderer.h"

class GuiComponent;

class GuiManager : public GameComponent, public BasicManager
{
public:
	GuiManager(Game* game);
	virtual ~GuiManager(void);

	virtual void init();

	virtual void draw(const GameTime& gameTime);
	virtual void update(const GameTime& gameTime);
	
	virtual void release();

	virtual void onLostDevice();
	virtual void onResetDevice(const GraphicsDevice& device);

	Rocket::Core::Context& getContext() const;

	bool isVisible() const { return visible; }
	void setVisible(bool visible) { this->visible = visible; }

protected:
	virtual void loadFonts();
	virtual void loadFont(const std::string& path);

private:
	void initContext();

	GuiSystem guiSystem;
	GuiRenderer guiRenderer;

	Rocket::Core::Context* context;

	Matrix projection;

	bool deviceWasLost;
	bool visible;
};