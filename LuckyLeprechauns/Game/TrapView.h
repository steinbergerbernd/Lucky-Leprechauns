#pragma once
#include "LLGraphicsComponent.h"
#include "TrapManager.h"

class TrapView : public LLGraphicsComponent
{
	typedef std::vector<Matrix> InstanceCollection;

public:
	TrapView(LLGraphicsManager* manager, const TrapManager* trapManager);

	void init();
	void draw(const GameTime& gameTime);

private:
	const TrapManager* trapManager;
	Vector3 enemyColor;

	FBXModel model;

	InstanceCollection instances;
};

