#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Rocket\Core.h"

#include "BasicComponent.h"
#include "GuiManager.h"

class GuiComponent : public BasicComponent<GuiManager>
{
typedef Rocket::Core::String RocketString;

public:
	GuiComponent(GuiManager* guiManager);
	virtual ~GuiComponent(void);

	virtual void draw(const GameTime& gameTime);

	virtual void onLostDevice();
	virtual void onResetDevice(const GraphicsDevice& device);

	std::string getElementText(const std::string& elementId) const;
	std::string getElementProperty(const std::string& elementId, const std::string& propertyName) const;

	void setElementText(const std::string& elementId, const std::string& text);
	void setElementProperty(const std::string& elementId, const std::string& propertyName, const std::string& propertyValue);
	void setElementClass(const std::string& elementId, const std::string& classValue);

protected:
	void loadDocument(const std::string& documentPath);

private:
	void loadDocument();

	const RocketString convert(const std::string& value) const;
	const std::string convert(RocketString value) const;

	Rocket::Core::ElementDocument* document;
	RocketString documentPath;
};

