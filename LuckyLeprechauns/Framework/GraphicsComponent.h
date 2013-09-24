#pragma once

#include "BasicComponent.h"
#include "GraphicsManager.h"

class GraphicsComponent : public BasicComponent<GraphicsManager>
{
public:
	GraphicsComponent(GraphicsManager* graphicsManager);
	virtual ~GraphicsComponent() = 0;

	virtual void draw(const GameTime& gameTime);

	void setTransparency(bool transparency) { this->transparency = transparency; }
	bool hasTransparency() const { return transparency; }

private:
	bool transparency;
};