#pragma once

#include "GraphicsTypes.h"
#include "FBXMaterial.h"

namespace FBXColorFunction
{
	enum : unsigned
	{
		colorMultiply = 0,
		colorAdd = 1,
		colorOverride = 2
	};
};

struct FBXMesh
{
	typedef std::vector<FBXMaterial> MaterialCollection;

	Mesh mesh;
	MaterialCollection materials;
	std::string name;

	bool visible;
	Vector3 color;
	unsigned colorFunction;

	bool colorOverride;
	bool colorAdd;

	FBXMesh(bool visible = true, Vector3 color = Vector3::one, unsigned colorFunction = FBXColorFunction::colorMultiply) : visible(visible), color(color), colorFunction(colorFunction) {}

	FBXMesh(Mesh mesh, MaterialCollection materials, std::string name, bool visible = true, Vector3 color = Vector3::one, unsigned colorFunction = FBXColorFunction::colorMultiply) 
		: mesh(mesh), materials(materials), name(name), visible(visible), color(color), colorFunction(colorFunction) {}
};