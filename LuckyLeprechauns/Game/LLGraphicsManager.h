#pragma once

#include "GraphicsManager.h"
#include "LuckyLeprechauns.h"

class TerrainView;
class PlayerView;
class SkyboxView;
class VegetationView;
class PigView;
class MushroomView;
class ItemView;
class RainbowView;
class GoldpotView;
class TrapView;

class LLGraphicsManager : public GraphicsManager
{
public:
	typedef std::list<MushroomView*> MushroomCollection;

	LLGraphicsManager(LuckyLeprechauns* game);

	void init();
	void release();
	void update(const GameTime& gameTime);
	void draw(const GameTime& gameTime);

	void drawShadowMap(const GameTime& gameTime);
	void drawLightMap(const GameTime& gameTime);

	void onResetDevice(const GraphicsDevice& device);
	void onLostDevice();

private:
	void set(const Matrix& view, const Plane& clipPlane, unsigned long cullmode, const Surface& renderTarget, const Surface& zBuffer);

	Surface backBuffer;
	Surface zBuffer;

	Surface shadowMapZBuffer;
	Surface shadowMapSurface;

	ComponentCollection refractionComponents;
	ComponentCollection shadowComponents;

	TerrainView* terrain;
	PlayerView* player;
	SkyboxView* skybox;
	VegetationView* vegetation;
	PigView* pig;
	ItemView* items;
	TrapView* traps;
	GoldpotView* goldpot;

	MushroomCollection mushrooms;

	float nearPlaneDistance;
	float farPlaneDistance;
};
