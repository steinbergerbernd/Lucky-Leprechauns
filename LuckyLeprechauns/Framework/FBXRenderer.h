#pragma once

#define WIN32_LEAN_AND_MEAN

#include "STL.h"
#include "GameTime.h"
#include "FBXModel.h"

class GraphicsComponent;
class GraphicsManager;

class FBXRenderer
{
public:
	static void draw(FBXModel& model, const GameTime& gameTime, const GraphicsManager& manager, const Matrix& world, bool transparency = false, unsigned long blendFactor = 0x000000, bool cullNone = false, bool disableLighting = false);
	static void draw(FBXModel& model, const GameTime& gameTime, const GraphicsManager& manager, const Matrix& world, FBXEffect& effect);
	static void draw(FBXModel& model, const GameTime& gameTime, const GraphicsManager& manager, const Matrix& world, const std::vector<Matrix>& instanceTransforms, unsigned numInstances);

private:
	static void prepare(FBXModel& model, const GraphicsManager& manager, bool transparency = false, unsigned long blendFactor = 0x000000, bool cullNone = false, bool disableLight = false);
	static void cleanup(const GraphicsManager& manager, bool transparency = false, bool cullNone = false);

	static unsigned long prevCullMode;
};