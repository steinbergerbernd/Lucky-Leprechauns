#pragma once

#include "Vector3.h"
#include "Color.h"


struct VertexPosition
{
	Vector3 position;

	static const unsigned long fvf;
};


struct VertexPositionTexture
{
	Vector3 position;
	Vector2 texCoord;

	static const unsigned long fvf;
};


struct VertexPositionColor
{
	Vector3 position;
	Color color;

	static const unsigned long fvf;
};


struct VertexPositionNormal
{
	Vector3 position;
	Vector3 normal;

	static const unsigned long fvf;
};

struct VertexPositionNormalTexture
{
	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;

	static const unsigned long fvf;
};

struct VertexPositionColorTexture
{
	Vector3 position;
	Color color;
	Vector2 texCoord;

	static const unsigned long fvf;
};