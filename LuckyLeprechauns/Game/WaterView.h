#pragma once

#include "LLGraphicsComponent.h"

#include "TerrainWaterEffect.h"

class TerrainView;

class WaterView : public LLGraphicsComponent
{
	typedef VertexPositionTexture TerrainWaterVertex;

public:
	WaterView(LLGraphicsManager* manager, TerrainView* terrain);

	void init();
	void draw(const GameTime& gameTime);

	const Surface& getZBuffer() const { return zBuffer; }

	const Surface& getReflectionSurface() const { return reflectionSurface; }
	const Surface& getRefractionSurface() const { return refractionSurface; }

	const Plane& getReflectionPlane() const { return reflectionPlane; }
	const Plane& getRefractionPlane() const { return refractionPlane; }

	void onLostDevice();
	void onResetDevice(const GraphicsDevice& device);

private:
	Texture reflectionmap;
	Texture refractionmap;

	Surface reflectionSurface;
	Surface refractionSurface;
	
	Plane reflectionPlane;
	Plane refractionPlane;

	Surface zBuffer;

	VertexBuffer vertexBuffer;

	TerrainWaterEffect effect;

	TerrainView* terrain;
};