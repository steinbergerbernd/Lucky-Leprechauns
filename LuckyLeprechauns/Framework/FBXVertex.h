#pragma once

#include "Types.h"

struct FBXVertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
	Vector4 blendWeights;
	unsigned char blendIndices[4];

	static const D3DVERTEXELEMENT9 vertexElements[];
};