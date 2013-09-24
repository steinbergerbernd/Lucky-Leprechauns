#pragma once

#include "LLControlledGraphicsComponent.h"
#include "LeprechaunController.h"

#include "CaneView.h"
#include "HatView.h"
#include "BubbleView.h"
#include "EyeView.h"

class LeprechaunView : public LLControlledGraphicsComponent<LeprechaunController>
{
public:
	LeprechaunView(LLGraphicsManager* manager, LeprechaunController* controller);

	void init();
	void update(const GameTime& gameTime);
	void draw(const GameTime& gameTime);

	HatView& getHat() { return *hat; }

private:
	FBXModel model;

	CaneView* cane;
	HatView* hat;
	BubbleView* bubble;
	EyeView* eye;

	Vector3 caneScaleOffset;
	Vector3 hatScaleOffset;

	std::string getCurrentAnimation() const;
};

